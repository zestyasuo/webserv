#ifndef Webserv_exception_HPP
#define Webserv_exception_HPP
#include "log_levels.hpp"
#include <iostream>

class Webserv_exception : public std::exception
{
  protected:
	int			error_code;
	const char *error_message;

  public:
	Webserv_exception(const char *, int);
	Webserv_exception(void);
	virtual int			get_error_code(void) const;
	virtual const char *what() const throw();
};

#endif
