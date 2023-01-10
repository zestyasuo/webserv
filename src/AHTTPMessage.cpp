#include "../inc/AHTTPMessage.hpp"

AHTTPMessage::AHTTPMessage()
{
	std::cout << "Def AHTTPMessage constructor\n";
}

AHTTPMessage::AHTTPMessage(AHTTPMessage const &copy)
{
	std::cout << "AHTTPMessage copy constructor";
	*this = copy;
}

AHTTPMessage::~AHTTPMessage(){ std::cout << "AHTTPMessage destructor\n";}

std::ostream	&operator<<(std::ostream &os, AHTTPMessage const &rhs)
{
	os << "I am AHTTPMessage. I reside on the adress: " << &rhs;
	return (os);
}

// AHTTPMessage	&AHTTPMessage::operator=(AHTTPMessage const &rhs)
// {
// 	std::cout << " = operator\n";
// 	if (this != &rhs)
// 	{

// 	}
// 	return (*this);
// }
