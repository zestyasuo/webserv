#include "../inc/HTTPResponse.hpp"
#include "../inc/Server.hpp"
#include <iostream>
#include <fstream>
#include <sys/stat.h>	//	S_IFDIR
#include <sys/wait.h>	//	S_IFDIR
#include <vector>
#define CRLF "\r\n"

using std::string;
using std::vector;

#define BUF_SIZE 4096

vector<char> cgi_exec(const string &fname)
{
	int fd[2];
	int cgi_pid;
	vector<char> cgi_data;
	char cgi_buf[BUF_SIZE];

	string cgi_path = "/usr/bin/python3";
//	string cgi_path = "/bin/echo";

	pipe(fd);
	cgi_pid = fork();
	if (cgi_pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], 1);
		execl(cgi_path.c_str(), "", fname.c_str());
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
	fname.append(req->get_target());

	status_code = 200;

//	resp = version + " " + "status_code" + " " + status_text + CRLF + CRLF;

	struct stat st;
	if (stat(fname.c_str(), &st) != 0)
	{
		status_code = 404;
		resp = version + " " + SSTR(status_code) + " " + status_text + CRLF + CRLF
			+ "<h3>404 - not found!</h3>";
		return resp;
	}

	if (st.st_mode & S_IFDIR)
	{
		//	check for index (config entry and file exist)
		resp += "DIR LISTING: " + fname + CRLF;
	}
	else if (open_fstream(fname, ifs) == 0)
	{
		if (fname.compare(fname.size() - 3, 3, ".py") == 0)
		{	//	CGI
			vector<char> cgi_data = cgi_exec(fname);
			content_type = "text/plain";
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