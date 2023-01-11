#include "../inc/HTTPReqest.hpp"

HTTPReqest::HTTPReqest()
{
	std::cout << "Def HTTPReqest constructor\n";
}

HTTPReqest::HTTPReqest(std::string const &raw) : AHTTPMessage(raw)
{
	(void) raw;
	(void) version;
	(void) target;
	(void) method;
	(void) headers;
	(void) body;
}

HTTPReqest::HTTPReqest(HTTPReqest const &copy)
{
	std::cout << "HTTPReqest copy constructor";
	*this = copy;
}

HTTPReqest::~HTTPReqest(){}

std::ostream	&operator<<(std::ostream &os, HTTPReqest const &rhs)
{
	os << "" << &rhs;
	return (os);
}

HTTPReqest	&HTTPReqest::operator=(HTTPReqest const &rhs)
{
	std::cout << " = operator\n";
	if (this != &rhs)
	{

	}
	return (*this);
}

std::string HTTPReqest::parse_version(std::string const &meta) const
{
	(void) meta;
	return (0);
}

std::vector<std::string> HTTPReqest::parse_headers(std::string const &meta) const
{
	(void) meta;
	std::vector<std::string>	vec;
	return (vec);
}