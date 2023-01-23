#include "../inc/HTTPResponse.hpp"
#include "../inc/Server.hpp"
// #include "../inc/Config_proto.hpp"
#include <iostream>
#include <sys/stat.h>	//	S_IFDIR
#include <sys/wait.h>	//	S_IFDIR
#include <vector>
#define CRLF "\r\n"

using std::string;
using std::vector;

#define BUF_SIZE 4096

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

bool	HTTPResponse::isMethodAllowed(s_location const &loc)
{
	std::string const &method = request->get_method();
	int					mask;

	mask = get_method_mask(method);
	if (loc.methods & mask)
		return true;
	return false;
}

s_location	const &get_location(std::string const &target, std::map<std::string, s_location> const &locations)
{
	std::string	to_find;

	if (target.find_last_of("/") != target.npos)
		to_find  = target.substr(0, target.find_last_of("/") + 1);

	return locations.at(to_find);
}

HTTPResponse::HTTPResponse(const HTTPRequest *req, t_conf const &conf) : request(req), config(conf)
{
	/*	hardcoded for debug	only!	*/
	version = "HTTP/1.1";
	status_code = 200;
	status_text = "OK";
	s_location	loc;
	std::string resp;
	std::string fname;

	try
	{
		loc = get_location(request->get_target(), config.locations);
	}
	catch(const std::exception& e)
	{
		status_code = 404;
		status_text = "NOT FOUND";
		return ;
	}
	fname = loc.root + request->get_target();
	if (!isMethodAllowed(loc))
	{
		status_code = 501;
		status_text = "NOT IMPLEMENTED";
		return ;
	}
	get_file_info(fname);
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

void	HTTPResponse::get_file_info(std::string const &fname)
{
	std::ifstream ifs;
	// string query_string;
	struct stat st;
	std::cout << fname;
	if (stat(fname.c_str(), &st) != 0)
	{
		status_code = 404;
		return ;
	}

	if (st.st_mode & S_IFDIR)
	{
		//	check for index (config entry and file exist)
		// status_text += "DIR LISTING: " + fname + CRLF;
		status_code = 501;
		std::cout << "Dir listing requeried\n";
		return ;
	}
	else if (open_fstream(fname, ifs) != 0)
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

void	HTTPResponse::read_file(std::ifstream &ifs)
{
		size_t resp_headers_size = payload.size();
		ifs.seekg(0, std::ios::end);
		size_t fsize = ifs.tellg();
		payload.resize(payload.size() + fsize);
		ifs.seekg(0);
		ifs.read(&payload[resp_headers_size], fsize);
}


std::string HTTPResponse::dump()
{
	// std::string fname;

	if (status_code == 404)
	{
		payload = version + " " + SSTR(status_code) + " " + status_text + CRLF + CRLF
			+ "<html><h3>404 - not found!</h3></html>";
		return payload;
	}
	if (status_code == 501)
	{
		payload = version + " " + SSTR(status_code) + " " + status_text + CRLF + CRLF
			+ "<html><h3>501 - not implemented!</h3></html>";
		return payload;
	}

	// fname.append(request->get_target());
	// size_t query_pos = fname.find('?');
	// if (query_pos != fname.npos)
	// {
		// query_string = fname.substr(fname.find('?') + 1, fname.length());
		// fname.erase(fname.find('?'));
	// }

	status_code = 200;
	content_type = "text/html";

//	resp = version + " " + "status_code" + " " + status_text + CRLF + CRLF;

	headers = version + " " + SSTR(status_code) + " " + status_text + CRLF;
	if (!content_type.empty())
		headers += "content-type:" + content_type + CRLF;
	headers += CRLF;
	payload.insert(0, headers);

	return payload;
}

HTTPResponse::HTTPResponse(void)
{}

HTTPResponse::HTTPResponse(HTTPResponse const &copy) : AHTTPMessage(), config(copy.config)
{
	*this = copy;
}

HTTPResponse::~HTTPResponse(){}

std::ostream	&operator<<(std::ostream &os, HTTPResponse const &rhs)
{
	(void) rhs;
	return (os);
}

HTTPResponse	&HTTPResponse::operator=(HTTPResponse const &rhs)
{
	if (this != &rhs)
	{

	}
	return (*this);
}

std::string	HTTPResponse::parse_version(std::vector<std::string> const &status_line) const
{
	(void) status_line;
	return (0);
}

std::map<std::string, std::string> HTTPResponse::parse_headers(std::vector<std::string> const &meta) const
{
	(void) meta;
	std::map<std::string, std::string>	vec;
	return (vec);
}