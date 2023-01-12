#ifndef HTTPResponse_HPP 
# define HTTPResponse_HPP
# include <iostream>
# include "AHTTPMessage.hpp"

class HTTPResponse : public AHTTPMessage
{
private:
	std::string	status_code;
	std::string	status_text;
	std::string								parse_version(std::vector<std::string> const &) const;
	std::map<std::string, std::string>		parse_headers(std::vector<std::string> const &) const;
public:
	HTTPResponse(void);
	HTTPResponse(HTTPResponse const &copy);
	~HTTPResponse(void);
	HTTPResponse	&operator=(HTTPResponse const &rhs);
};

std::ostream	&operator<<(std::ostream &os, HTTPResponse const &rhs);
#endif