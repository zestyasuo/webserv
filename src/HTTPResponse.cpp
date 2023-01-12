#include "../inc/HTTPResponse.hpp"
#include "../inc/Server.hpp"
#define CRLF "\r\n"

HTTPResponse::HTTPResponse()
{

}

HTTPResponse::HTTPResponse(const HTTPRequest *req)
{
	(void)req;
	/*	hardcoded for debug	only!	*/
	version = "HTTP/1.1";
	status_code = 200;
	status_text = "OK";
}

std::string HTTPResponse::dump()
{
	std::string	resp;

	resp = version + " " + "status_code" + " " + status_text + CRLF + CRLF;
	resp += "<h3>Test</h3>";
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