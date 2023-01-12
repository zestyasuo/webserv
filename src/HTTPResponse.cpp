#include "../inc/HTTPResponse.hpp"

HTTPResponse::HTTPResponse()
{
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