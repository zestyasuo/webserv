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
public:
	AHTTPMessage(void);
	AHTTPMessage(AHTTPMessage const &copy);
	virtual ~AHTTPMessage(void) = 0;
};

#endif