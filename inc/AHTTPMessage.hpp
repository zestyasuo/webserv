#ifndef AHTTPMessage_HPP 
# define AHTTPMessage_HPP
# include <iostream>
# include <vector>
# include "utils.hpp"

class AHTTPMessage
{
protected:
	std::string					version;
	std::vector<std::string>	headers;
	std::string					body;
private:
	std::string							raw_data;
	virtual std::string						parse_version(std::string const &) const = 0;
	virtual std::vector<std::string>		parse_headers(std::string const &) const = 0;
	virtual std::string						parse_body(std::string const &) const;
public:
	std::string	const &get_raw_data(void) const;
	AHTTPMessage(AHTTPMessage const &copy);
	AHTTPMessage(void);
	AHTTPMessage(std::string const &);
	virtual ~AHTTPMessage(void);
};

#endif