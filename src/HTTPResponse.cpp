#include "../inc/HTTPResponse.hpp"
#include "../inc/Server.hpp"
#include "../inc/Config_proto.hpp"
#include <iostream>
#include <fstream>
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

HTTPResponse::HTTPResponse()
{

}

HTTPResponse::HTTPResponse(const HTTPRequest *req) : req(req)
{
	(void)req;
	/*	hardcoded for debug	only!	*/
	version = "HTTP/1.1";
	status_code = 200;
	status_text = "OK";
}

int open_fstream(const string &fname, std::ifstream &ifs)
{
	ifs.open(fname.c_str(), std::ios::binary);
	if (!ifs.good())
		return -1;
	return 0;
}

std::string HTTPResponse::dump()
{
	std::string	resp;
	std::ifstream ifs;
	string	fname = "/home/sname/ws/www/serv_a";

	string query_string;

	fname.append(req->get_target());
	size_t query_pos = fname.find('?');
	if (query_pos != fname.npos)
	{
		query_string = fname.substr(fname.find('?') + 1, fname.length());
		fname.erase(fname.find('?'));
	}

	status_code = 200;
	content_type = "text/html";

//	resp = version + " " + "status_code" + " " + status_text + CRLF + CRLF;

	struct stat st;
	if (stat(fname.c_str(), &st) != 0)
	{
		status_code = 404;
		resp = version + " " + SSTR(status_code) + " " + status_text + CRLF + CRLF
			+ "<html><h3>404 - not found!</h3></html>";
		return resp;
	}

	if (st.st_mode & S_IFDIR)
	{
		//	check for index (config entry and file exist)
		resp += "DIR LISTING: " + fname + CRLF;
	}
	else if (open_fstream(fname, ifs) == 0)
	{
//		if (fname.compare(fname.size() - 3, 3, ".py") == 0)
		if (fname.compare(fname.size() - 4, 4, ".php") == 0)
		{	//	CGI
			query_string.insert(0, "QUERY_STRING=");
			vector<char> cgi_data = cgi_exec(fname, query_string);
			resp.append(cgi_data.data(), cgi_data.size());
		}
		else
		{	//	common files
			size_t resp_headers_size = resp.size();
			ifs.seekg(0, std::ios::end);
			size_t fsize = ifs.tellg();
			resp.resize(resp.size() + fsize);
			ifs.seekg(0);
			ifs.read(&resp[resp_headers_size], fsize);
		}
	}
	else
		status_code = 404;
//		resp += "404" CRLF;

	headers = version + " " + SSTR(status_code) + " " + status_text + CRLF;
	if (!content_type.empty())
		headers += "content-type:" + content_type + CRLF;
	headers += CRLF;
	resp.insert(0, headers);

	return resp;
}

HTTPResponse::HTTPResponse(HTTPResponse const &copy) : AHTTPMessage()
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