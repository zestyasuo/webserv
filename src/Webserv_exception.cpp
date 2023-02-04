#include "../inc/Webserv_exception.hpp"

Webserv_exception::Webserv_exception()
	: error_code(-1), error_message("Not defined error")
{
}

Webserv_exception::Webserv_exception(const char *message, int code)
	: error_code(code), error_message(message)
{
}

const char *Webserv_exception::what() const throw()
{
	return error_message;
}

int Webserv_exception::get_error_code(void) const
{
	return error_code;
}
