#ifndef Logger_HPP
#define Logger_HPP
#include "Colors.hpp"
#include "Webserv_exception.hpp"
#include "log_levels.hpp"
#include <iostream>
#include <string>

class Logger
{
  private:
	static const std::string log_level_names[DEBUG + 1];
	std::string				 make_logstr(std::string const &msg) const;
	bool					 is_debug;
	std::string				name;
	bool	is_active;

  public:
	void	deactivate(void);
	std::string get_timestamp(void) const;
	Logger(void);
	Logger(bool, std::string const &);
	Logger(bool);
	~Logger(void);
	void log(std::string, int) const;
};

#endif
