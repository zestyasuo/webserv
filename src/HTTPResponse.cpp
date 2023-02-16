#include "../inc/HTTPResponse.hpp"
#include "../inc/Server.hpp"
#include "../inc/file_utils.hpp"
#include "Config.hpp"
#include "log_levels.hpp"
#include "utils.hpp"
#include <fstream>
#include <string>
#include <vector>
// #include "../inc/Config.hpp"


using std::string;
using std::vector;


// utils



vector< char > HTTPResponse::cgi_exec()
{
	int			   fd[2];
	int			   cgi_pid = 0;
	vector< char > cgi_data;
	char		   cgi_buf[CGI_BUFF_SIZE];
	
	// std::cout << "\tFNAME: " << fname << "\n";

	//	string cgi_path = "/usr/bin/python3";
//	string cgi_path = "/bin/php8.1";

	const string cgi_path = config.cgi.at(request_file_ext);

	pipe(fd);
	cgi_pid = fork();
	if (cgi_pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], 1);
		//		execl(cgi_path.c_str(), NULL, fname.c_str());
		char *envp[] = {(char *)"TEST=Value", (char *)"VAR=val", (char *)cgi_query_str.c_str(), 0};
		//		extern t_conf g_conf;

		execle(cgi_path.c_str(), "", request_full_path.c_str(), NULL, envp);
		exit(0);
	}
	close(fd[1]);
	wait(&cgi_pid);

	size_t bytes_read = 0;
	bytes_read = read(fd[0], cgi_buf, CGI_BUFF_SIZE);
	while (bytes_read > 0)
	{
		cgi_data.insert(cgi_data.end(), cgi_buf, cgi_buf + bytes_read);
		bytes_read = read(fd[0], cgi_buf, CGI_BUFF_SIZE);
	}
	close(fd[0]);
	return cgi_data;
}

/// @brief returns method mask based on method name or 0 if method not supported
/// @param str - method name
/// @return method mask or 0 if not found
int get_method_mask(std::string const &str)
{
	int mask = 0;

	// head method plug (tester says "bad status code")
	if (str == "GET")
		mask |= em_get;
	else if (str == "POST")
		mask |= em_post;
	else if (str == "DELETE")
		mask |= em_delete;
	else if (str == "PUT")
		mask |= em_put;
	else if (str == "HEAD")
		mask |= em_head;

	return mask;
}

int open_fstream(const string &fname, std::ifstream &ifs)
{
	ifs.open(fname.c_str(), std::ios::binary);
	if (!ifs.good())
		return -1;
	return 0;
}

bool HTTPResponse::is_cgi()
{
	return (config.cgi.count(request_file_ext));

//	if (fname != "")
//		return false;
//	return false;
}

std::vector<std::string> make_possible_loc_list(std::string const &target)
{
	std::string	to_find = "/";
	std::vector<std::string> res;
	std::vector<std::string> split_target = split(target, "/");

	for (std::vector<std::string>::iterator it = split_target.begin(); it != split_target.end(); it++)
	{
		to_find += *it;
		res.push_back(to_find);
		to_find += "/";
	}
	return res;
}

/// @brief looks for a location struct specified by config.
/// @param target needed location
/// @param locations full list of locations from config
/// @return specified location or defualt location
s_location const get_location(std::string const &target, std::map< std::string, s_location > const &locations)
{
	std::string to_find = "";
	std::vector<std::string> possible_locations = make_possible_loc_list(target);
	s_location loc = locations.at("/");

	for (std::vector<std::string>::reverse_iterator it = possible_locations.rbegin(); it != possible_locations.rend(); it++)
	{
		if (locations.count(*it))
			loc = locations.at(*it);
	}
	return loc;
}

/// @brief tries to get index pages specified from config. If none specified
/// tries index.html
/// @param fname targeted file from request
/// @param loc location of request
/// @return 0 if page was found. 1 if not
int HTTPResponse::try_index_page(std::string const &fname, s_location const &loc)
{
	std::vector< std::string > copy = loc.index_files;

	if (copy.empty())
		copy.push_back("index.html");
	for (std::vector< std::string >::iterator it = copy.begin(); it != copy.end(); it++)
	{
		get_file_info(fname + (fname.c_str()[fname.length()] == '/' ? "" : "/") + (*it));
		if (status_code == 200)
			return 0;
	}
	return 1;
}

// constructors

HTTPResponse::HTTPResponse(void) : status_code(), request(), config(), logger()
{
}

HTTPResponse::HTTPResponse(HTTPResponse const &copy)
	: AHTTPMessage(copy), version(copy.version), status_code(copy.status_code), content_type(copy.content_type), body(copy.body),
	  request(copy.request), config(copy.config), logger(copy.logger)
{
}

HTTPResponse::~HTTPResponse()
{
}

/// @brief makes dumpable response object. the only way to get a response
/// @param req parsed request
/// @param conf parsed config
HTTPResponse::HTTPResponse(const HTTPRequest *req, t_conf const &conf)
	: status_code(0), status_text(""), request(req), config(conf), error_pages(config.error_pages), status_texts(config.status_texts), logger(true, "Response")
{
	if (!request)
		return;
	content_type = CTYPE_TEXT_HTML;
	s_location	loc;
	std::string resp;
	std::string fname;
	std::string root;

	version = req->get_version();
	if (!request->is_valid())
	{
		status_code = 400;
		ready_up();
		return;
	}
	loc = get_location(request->get_target(), config.locations);
	if (!loc.rewrite.empty())
	{
		status_code = 301;
		add_header("Location", loc.rewrite);
		ready_up();
		return;
	}
	// std::cout << "path: " << loc.path << "\n";
	if (check_method(loc))
	{
		ready_up();
		return;
	}
	root = loc.root.empty() ? config.root : loc.root;
	// std::cout << "root: " << root << std::endl;
	std::string target = request->get_target();
	target.erase(target.find(loc.path), loc.path.length());
	fname = root + (target.c_str()[0] == '/' ? "" : "/") + target;
	// fname = root + "/" + request_full_path;
	
	request_file_ext = get_path_ext(fname);

	split_query(fname, request_full_path, cgi_query_str);
	decode_html_enities(request_full_path);

	// std::cout << "request_full_path : '" << request_full_path << "'\n";
	logger.log("target: " + SSTR(target), DEBUG);
	// std::cout << "cgi_query_str : '" << cgi_query_str << "'\n";
	// std::cout << "request_file_ext : '" << request_file_ext << "'\n";

	process_target(request_full_path, loc);
	ready_up();
}

// not needed yet, name reserved
std::string HTTPResponse::parse_target(std::string const &target, std::string const &loc_path)
{
	std::string copy = target + loc_path;

	return copy;
}

int HTTPResponse::check_method(s_location const &loc)
{
	std::string method = request->get_method();
	int			mask = get_method_mask(method);

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

/// @brief tries to open target file and sets status code, headers and messages
/// to appropriate state
/// @param fname target
/// @param loc target location
///	fname_raw may contain html entities (e.g. %20 = ' ')
///	which converted to their single character values in fname

void HTTPResponse::process_target(std::string const &fname_raw, s_location const &loc)
{
	string fname(fname_raw);
	decode_html_enities(fname);
	struct stat st = {};
	std::string method = request->get_method();
	std::string	filename = fname.substr(fname.find_last_of("/"));

	if (stat(fname.c_str(), &st) != 0)
	{
		if ((get_method_mask(method) & em_put) == em_put && loc.is_upload_allowed)
		{
			create_file_and_write_contents(loc.upload_path + filename, request->get_body());
		}
		else
		{
			std::cout << "Not found in stat  " << fname << "\n";
			status_code = 404;
		}
		return;
	}

	//potential_error cause em_post and em_get behave the same, double check
	if (st.st_mode & S_IFDIR && get_method_mask(method) & (em_get | em_head | em_post))
	{
		// std::cout << "DIR TRY\n";
		if (try_index_page(fname, loc) != 0)
		{
			content_type = CTYPE_TEXT_HTML;
			status_code = 200;
			body += dir_list_formatted(fname, this->request, true);
			wrap_html_body(body);
		}
	}
	else
	{
		if (get_method_mask(method) & (em_get | em_head | em_post))
			get_file_info(fname);
		else if (get_method_mask(method) & em_delete)
			delete_file(fname);
		// else if (get_method_mask(method) & em_post)
			// hadndle_post(fname);
	}
}

void	HTTPResponse::hadndle_post(std::string const &fname)
{
	status_code = 200;
	content_type = CTYPE_TEXT_HTML;
	std::cout << "post method " <<fname << "\n";
}

void	HTTPResponse::create_file_and_write_contents(std::string const &fname, std::string const &content)
{
	std::ofstream ofs(fname.c_str());

	if (!ofs.good())
	{
		status_code = 500;
		return;
	}
	ofs << content;
	ofs.close();
}

void HTTPResponse::delete_file(std::string const &fname)
{
	if (std::remove(fname.c_str()))
		status_code = 500;
	else
		status_code = 200;
}

/// @brief opens file by path. reads it if found and method is get. deletes it
/// if found and method is delete. todo: check if method is put and do something
/// @param fname file path
void HTTPResponse::get_file_info(std::string const &fname)
{
	std::ifstream ifs;
	// string query_string;
	std::string	const method = request->get_method();

	if (open_fstream(fname, ifs) != 0)
	{
		status_code = 404;
		// std::cout << "Not found in opening file " << fname << "\n";
		return;
	}
	if (is_cgi())
	{
		std::cout << "I am cgi " << request_file_ext << "\n";
		content_type = "text/html";

		std::vector<char> cgi_data (cgi_exec());

		body.insert(body.begin(), cgi_data.begin(), cgi_data.end());
		status_code = 200;
	}
	else
	{
		status_code = 200;
		read_file(ifs);
		// ???; process partial put -> 400 BAD REQUEST
	}

	ifs.close();
}

/// @brief reads file from stream to payload
/// @param ifs file stream
void HTTPResponse::read_file(std::ifstream &ifs)
{
	size_t resp_headers_size = body.size();
	ifs.seekg(0, std::ios::end);
	ssize_t fsize = ifs.tellg();
	body.resize(body.size() + fsize);
	ifs.seekg(0);
	ifs.read(&body[resp_headers_size], fsize);
}

void	HTTPResponse::add_body(void)
{
	std::string html;

	if (error_pages.count(status_code))
		html = error_pages.at(status_code);
	body.insert(0, html);
	payload += body;
}

void	HTTPResponse::add_meta_data(void)
{
	std::string status_line;

	status_text = get_status_message_by_code(status_code);
	status_line = version + " " + SSTR(status_code) + " " + status_text + LB;
	add_header("Date", get_floctime());
	add_header("Content-Type", content_type);
	std::string headers_str = map_to_str(headers);
	meta_data += status_line + headers_str;

	payload += meta_data;
}

/// @brief prepare response for writing and sending
/// @param  nothing
void HTTPResponse::ready_up(void)
{
	add_meta_data();
	if ((get_method_mask(request->get_method() ) & em_head) != em_head)
		add_body();
}

/// @brief HTTPResponse to_string
/// @return HTTPResponse string representaion
std::string HTTPResponse::to_string() const
{
	// logger.log(payload, DEBUG);
	// logger.log("method; " + request->get_method(),DEBUG);
	return payload;
}

/// @brief operator for stream
/// @param os open stream
/// @param rhs object
/// @return stream
std::ostream &operator<<(std::ostream &os, HTTPResponse const &rhs)
{
	(void)(rhs);
	return (os);
}

std::string HTTPResponse::parse_version(std::vector< std::string > const &status_line) const
{
	(void)status_line;
	return (0);
}

std::map< std::string, std::string > HTTPResponse::parse_headers(std::vector< std::string > const &meta) const
{
	(void)meta;
	return headers;
}

void HTTPResponse::add_header(std::string const &key, std::string const &value)
{
	headers.insert(std::make_pair< std::string, std::string >(key, value));
}