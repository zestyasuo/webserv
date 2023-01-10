#include "../inc/HTTPReqest.hpp"

HTTPReqest::HTTPReqest()
{
	std::cout << "Def HTTPReqest constructor\n";
}

HTTPReqest::HTTPReqest(std::string const &raw)
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

HTTPReqest::~HTTPReqest(){ std::cout << "HTTPReqest destructor\n";}

std::ostream	&operator<<(std::ostream &os, HTTPReqest const &rhs)
{
	os << "I am HTTPReqest. I reside on the adress: " << &rhs;
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
