#include "../inc/HTTPResponse.hpp"
#include "../inc/Server.hpp"
// #include "../inc/Config_proto.hpp"

#define CRLF "\r\n"

using std::string;
using std::vector;

#define BUF_SIZE 4096

const	HTTPResponse::T HTTPResponse::response_bodies_pairs[] = {
	{200, ""},
	{501, "<html><h3>501 - not implemented!</h3></html>"},
	{404, "<html><h3>404 - not found!</h3></html>"},
	{405, "<html><h3>405 - method not allowed!</h3></html>"},
	{500, "<html><h3>500 - Internal Server Error!</h3></html>"}
};

const HTTPResponse::int_to_string_map_t HTTPResponse::response_bodies(response_bodies_pairs, response_bodies_pairs + sizeof(HTTPResponse::response_bodies_pairs) /sizeof (HTTPResponse::T));

const	HTTPResponse::T HTTPResponse::status_text_pairs[] = {
	{200, "OK"}, {501, "Not Implemented"}, {404, "Not Found"}, {405, "Method Not Allowed"}, {500, "Internal Server Error"}
};
const HTTPResponse::int_to_string_map_t HTTPResponse::status_texts(status_text_pairs, status_text_pairs + sizeof(HTTPResponse::status_text_pairs) /sizeof (HTTPResponse::T));

// utils

/// @brief Creates HTTP message compatable string with headers
/// @param m - map of headers
/// @return http message compatable headers
std::string	map_to_str(std::map<std::string, std::string> const &m)
{
	std::string	res;
	for (std::map<std::string, std::string>::const_iterator it = m.begin(); it != m.end(); it++)
	{
		res += (*it).first + ": " + (*it).second + CRLF;
	}
	return res;
}

vector<char> cgi_exec(const string &fname, const string &query_str)
{
	int fd[2];
	int cgi_pid;
	vector<char> cgi_data;
	char cgi_buf[BUF_SIZE];

//	string cgi_path = "/usr/bin/python3";
	string cgi_path = "/bin/php8.1";

	pipe(fd);
	cgi_pid = fork();
	if (cgi_pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], 1);
//		execl(cgi_path.c_str(), NULL, fname.c_str());
		char * envp[] =
			{
				(char*)"TEST=Value",
				(char*)"VAR=val",
				(char*)query_str.c_str(),
			0
			};
//		extern t_conf g_conf;

		execle(cgi_path.c_str(), "", fname.c_str(), NULL, envp);
		exit(0);
	}
	close(fd[1]);
	wait(&cgi_pid);

	size_t bytes_read = 0;
	bytes_read = read(fd[0], cgi_buf, BUF_SIZE);
	while (bytes_read > 0)
	{
		cgi_data.insert(cgi_data.end(), cgi_buf, cgi_buf + bytes_read);
		bytes_read = read(fd[0], cgi_buf, BUF_SIZE);
	}
	close (fd[0]);
	return cgi_data;
}

/// @brief returns method mask based on method name or 0 if method not supported
/// @param str - method name
/// @return method mask or 0 if not found
int		get_method_mask(std::string const &str)
{
	int		mask = 0;

	if (str == "GET")
		mask |= em_get;
	else if (str == "POST")
		mask |= em_post;
	else if (str == "DELETE")
		mask |= em_delete;
	
	return mask;
}

int open_fstream(const string &fname, std::ifstream &ifs)
{
	ifs.open(fname.c_str(), std::ios::binary);
	if (!ifs.good())
		return -1;
	return 0;
}

bool	is_cgi(std::string const &fname)
{
	if (fname != "")
		return false;
	return false;
}

/// @brief looks for a location struct specified by config.
/// @param target needed location
/// @param locations full list of locations from config
/// @return specified location or throws std::out_of_range
s_location	const &get_location(std::string const &target, std::map<std::string, s_location> const &locations)
{
	std::string	to_find;

	if (target.find_last_of("/") != target.npos)
		to_find  = target.substr(0, target.find_last_of("/"));
	std::cout << "to_find: " << to_find << "\n";
	s_location const &loc = locations.count(to_find) ? locations.at(to_find) : locations.at("/");
	return loc;
}

/// @brief tries to get index pages specified from config. If none specified tries index.html
/// @param fname targeted file from request
/// @param loc location of request
/// @return 0 if page was found. 1 if not
int	HTTPResponse::try_index_page(std::string const &fname, s_location const &loc)
{
	std::vector<std::string> copy = loc.index_files;

	if (copy.empty())
		copy.push_back("index.html");
	for (std::vector<std::string>::iterator it = copy.begin(); it != copy.end(); it++)
	{
		get_file_info(fname + (fname.c_str()[fname.length()] == '/' ? "" : "/") + (*it));
		if (status_code == 200)
			return 0 ;
	}
	return 1;
}

// constructors

HTTPResponse::HTTPResponse(void) : config()
{}

HTTPResponse::HTTPResponse(HTTPResponse const &copy) : 
				AHTTPMessage(copy),
				version(copy.version),
				status_code(copy.status_code),
				content_type(copy.content_type),
				payload(copy.payload),
				request(copy.request),
				config(copy.config)
{
}

HTTPResponse::~HTTPResponse(){}

/// @brief makes dumpable response object. the only way to get a response
/// @param req parsed request
/// @param conf parsed config
HTTPResponse::HTTPResponse(const HTTPRequest *req, t_conf const &conf) :
			status_code(0),
			status_text(""),
			request(req),
			config(conf)
{
	if (!req)
		return ;
	s_location	loc;
	std::string resp;
	std::string fname;

	version = req->get_version();
	loc = get_location(request->get_target(), config.locations);
	std::string target = request->get_target();
	std::cout << "location :" << config.root + loc.path << "\n";
	if (check_method(loc))
	{
		ready_up();
		return ;
	}
	// std::cout << "target: " << target << "\n";
	fname = config.root + target;
	process_target(fname, loc);
	// add_header("Location", "/");
	ready_up();
}

int		HTTPResponse::check_method(s_location const &loc)
{
	std::string method = request->get_method();
	int mask = get_method_mask(method);

	if ((mask & config.implemented_methods) == 0)
	{
		status_code = 501;
		return 1;
	}
	if ((loc.methods & mask) == 0)
	{
		status_code = 405;
		return 1;
	}
	return 0;
}

/// @brief tries to open target file and sets status code, headers and messages to appropriate state
/// @param fname target
/// @param loc target location
void	HTTPResponse::process_target(std::string const &fname, s_location const &loc)
{
	struct stat	st;
	std::string	method = request->get_method();

	std::cout << fname << "\n";

	if (stat(fname.c_str(), &st) != 0)
	{
		status_code = 404;
		return ;
	}

	if (st.st_mode & S_IFDIR)
	{
		std::cout << "DIR TRY\n";
		if (try_index_page(fname, loc) != 0)
		{
			status_code = 501;
			std::cout << "dir listing requered;\n";
		}
	}
	else
	{
		if (get_method_mask(method) & em_get)
			get_file_info(fname);
		else if (get_method_mask(method) & em_delete)
			delete_file(fname);
	}
}

void	HTTPResponse::delete_file(std::string const &fname)
{
	if (std::remove(fname.c_str()))
		status_code = 500;
	else
		status_code = 200;
}

/// @brief opens file by path. reads it if found and method is get. deletes it if found and method is delete. todo: check if method is put and do something
/// @param fname file path
void	HTTPResponse::get_file_info(std::string const &fname)
{
	std::ifstream ifs;
	// string query_string;

	if (open_fstream(fname, ifs) != 0)
	{
		// todo: is redirect
		// todo: is method put 
		status_code = 404;
		return ;
	}
	// if (fname.compare(fname.size() - 3, 3, ".py") == 0)
	if (is_cgi(fname))
	{	//	CGI
		//HTTPRespone::exec_cgi();
		status_code = 501; // заглушка
		std::cout << "exec cgi requered\n";
	}
	else
	{
		// todo: is method delete
		status_code = 200;
		// if (is_method_get())
		read_file(ifs);
		// if (is_method_delete())
			// delete_file(ifs);
		// if (is_method_put)
			// ???; process partial put -> 400 BAD REQUEST
	}

	ifs.close();
}

/// @brief reads file from stream to payload
/// @param ifs file stream
void	HTTPResponse::read_file(std::ifstream &ifs)
{
		size_t resp_headers_size = payload.size();
		ifs.seekg(0, std::ios::end);
		size_t fsize = ifs.tellg();
		payload.resize(payload.size() + fsize);
		ifs.seekg(0);
		ifs.read(&payload[resp_headers_size], fsize);
}

/// @brief prepare response for writing and sending
/// @param  nothing
void	HTTPResponse::ready_up(void)
{
	std::string	html;

	if (response_bodies.count(status_code))
		html = response_bodies.at(status_code);
	if (status_texts.count(status_code))
		status_text = status_texts.at(status_code);
	payload.insert(0, html);
	add_header("Date", get_floctime());
	add_header("content-type", "text/html");
	std::string headers_str = map_to_str(headers);
	payload.insert(0, headers_str + CRLF);
	insert_status_line();
}

/// @brief HTTPResponse to_string
/// @return HTTPResponse string representaion
std::string HTTPResponse::to_string() const
{
	// fname.append(request->get_target());
	// size_t query_pos = fname.find('?');
	// if (query_pos != fname.npos)
	// {
		// query_string = fname.substr(fname.find('?') + 1, fname.length());
		// fname.erase(fname.find('?'));
	// }
	return payload;
}

/// @brief inserts status line
/// @param none
void	HTTPResponse::insert_status_line(void)
{
	std::string status_line;

	status_line = version + " " + SSTR(status_code) + " " + status_text + CRLF;
	// if (!content_type.empty())
	// 	status_line += "content-type:" + content_type + CRLF;
	// status_line += CRLF;
	payload.insert(0, status_line);
}


/// @brief operator for stream
/// @param os open stream
/// @param rhs object
/// @return stream
std::ostream	&operator<<(std::ostream &os, HTTPResponse const &rhs)
{
	(void)(rhs);
	return (os);
}

std::string	HTTPResponse::parse_version(std::vector<std::string> const &status_line) const
{
	(void) status_line;
	return (0);
}

std::map<std::string, std::string>	HTTPResponse::parse_headers(std::vector<std::string> const &meta) const
{
	(void) meta;
	return headers;
}

void	HTTPResponse::add_header(std::string const &key, std::string const &value)
{
	headers.insert(std::make_pair<std::string, std::string>(key, value));
}