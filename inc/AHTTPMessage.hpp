#ifndef AHTTPMessage_HPP 
# define AHTTPMessage_HPP
# include <iostream>
# include <vector>
# include "utils.hpp"
# include <map>

class AHTTPMessage
{
protected:
	std::string							version;
	std::map<std::string, std::string>	headers;
	std::string							body;
private:
	std::string											raw_data;
	std::vector<std::string>							meta_data;
	virtual std::string								parse_version(std::string const &) const = 0;
	virtual std::map<std::string, std::string>		parse_headers(std::vector<std::string> const &) const = 0;
	std::string										parse_body(std::string const &) const;
public:
	std::string const							&get_body(void) const;
	std::map<std::string, std::string> const	&get_headers(void) const;
	std::string const							&get_version(void) const;
	std::string	const &get_raw_data(void) const;
	std::vector<std::string> const &get_meta_data(void) const;
	AHTTPMessage(AHTTPMessage const &);
	AHTTPMessage(void);
	AHTTPMessage(std::string const &);
	virtual ~AHTTPMessage(void);
};

#endif