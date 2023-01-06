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
