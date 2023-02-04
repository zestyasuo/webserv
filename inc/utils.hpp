#ifndef utils_HPP
#define utils_HPP

#include "Webserv_exception.hpp"
#include <algorithm>
#include <ctime>
#include <fcntl.h>
#include <map>
#include <vector>

void					   unblock_fd(int fd);
std::vector< std::string > split(std::string const &str,
								 std::string const &del);
std::string				   get_floctime(void);

template < class UnaryPredicate >
std::vector< std::string > split_if(std::string const &str, UnaryPredicate p)
{
	std::string				   token;
	std::string				   copy;
	std::vector< std::string > res;

	copy = str;
	std::string::iterator pos = copy.begin();
	while ((pos = std::find_if(copy.begin(), copy.end(), p)) != copy.end())
	{
		token = copy.substr(0, std::distance(copy.begin(), pos));
		if (token.length() == 0)
		{
			copy.erase(0, 1);
			continue;
		}
		res.push_back(token);
		while (!p(*(copy.begin())))
			copy.erase(0, 1);
	}
	if (find_if(copy.begin(), copy.end(), p) == copy.end() && copy != "")
		res.push_back(copy);

	return res;
}

#endif