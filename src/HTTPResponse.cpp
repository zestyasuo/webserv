#include "../inc/HTTPResponse.hpp"

HTTPResponse::HTTPResponse()
{
	std::cout << "Def HTTPResponse constructor\n";
}

HTTPResponse::HTTPResponse(HTTPResponse const &copy)
{
	std::cout << "HTTPResponse copy constructor";
	*this = copy;
}

HTTPResponse::~HTTPResponse(){ std::cout << "HTTPResponse destructor\n";}

std::ostream	&operator<<(std::ostream &os, HTTPResponse const &rhs)
{
	os << "I am HTTPResponse. I reside on the adress: " << &rhs;
	return (os);
}

HTTPResponse	&HTTPResponse::operator=(HTTPResponse const &rhs)
{
	std::cout << " = operator\n";
	if (this != &rhs)
	{

	}
	return (*this);
}

std::string HTTPResponse::parse_version(std::string const &meta) const
{
	(void) meta;
	return (0);
}

std::vector<std::string> HTTPResponse::parse_headers(std::string const &meta) const
{
	(void) meta;
	std::vector<std::string>	vec;
	return (vec);
}