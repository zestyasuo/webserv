#ifndef Logger_HPP 
# define Logger_HPP
# include "Colors.hpp"
# include <iostream>
# include <string>
# include "Webserv_exception.hpp"

class Logger
{
private:
	static const std::string log_level_names[DEBUG + 1];
public:
	Logger(void);
	~Logger(void);
	void	log(std::string, int) const;
};

#endif
