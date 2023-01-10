#ifndef AHTTPMessage_HPP 
# define AHTTPMessage_HPP
# include <iostream>
# include <vector>

class AHTTPMessage
{
protected:
	std::string					version;
	std::vector<std::string>	headers;
	std::string					body;
private:
	// virtual std::string					parse_version(std::string &) const = 0;
	// virtual std::vector<std::string>	parse_headers(std::string &) const = 0;
	// virtual std::string					parse_body(std::string &) const;
public:
	AHTTPMessage(void);
	AHTTPMessage(AHTTPMessage const &copy);
	virtual ~AHTTPMessage(void) = 0;
};

#endif