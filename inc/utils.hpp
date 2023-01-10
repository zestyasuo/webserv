#ifndef utils_HPP 
# define utils_HPP

# include "Webserv_exception.hpp"
# include <fcntl.h>
# include <vector>

void	unblock_fd(int fd);
std::vector<std::string> split(std::string const &str, std::string const &del);

#endif