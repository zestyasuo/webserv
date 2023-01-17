#ifndef HTTPResponse_HPP 
# define HTTPResponse_HPP
# include <iostream>
# include "AHTTPMessage.hpp"
# include "HTTPRequest.hpp"

class HTTPResponse : public AHTTPMessage
{
private:
	std::string	version;
	int			status_code;
	std::string	status_text;
	std::string	content_type;
	std::string	headers;
	std::string	payload;
	HTTPRequest	const *req;
	std::string								parse_version(std::vector<std::string> const &) const;
	std::map<std::string, std::string>		parse_headers(std::vector<std::string> const &) const;
public:
	HTTPResponse();
	explicit HTTPResponse(const HTTPRequest *);
	HTTPResponse(HTTPResponse const &copy);
	~HTTPResponse(void);
	std::string		dump();
	HTTPResponse	&operator=(HTTPResponse const &rhs);
};

std::ostream	&operator<<(std::ostream &os, HTTPResponse const &rhs);
#endif