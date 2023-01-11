#ifndef HTTPReqest_HPP 
# define HTTPReqest_HPP
# include "AHTTPMessage.hpp"
# include <iostream>

class HTTPReqest : public AHTTPMessage
{
private:
	std::string target;
	std::string method;
	virtual std::string						parse_version(std::string const &) const;
	virtual std::vector<std::string>		parse_headers(std::string const &) const;
public:
	HTTPReqest(void);
	HTTPReqest(std::string const &raw);
	HTTPReqest(HTTPReqest const &copy);
	~HTTPReqest(void);
	HTTPReqest	&operator=(HTTPReqest const &rhs);
};

std::ostream	&operator<<(std::ostream &os, HTTPReqest const &rhs);
#endif