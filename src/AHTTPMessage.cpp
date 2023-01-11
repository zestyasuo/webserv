#include "../inc/AHTTPMessage.hpp"

std::string	get_meta_data(std::string const &str)
{
	size_t			empty_line_index = str.find("\n\n");
	std::string		res;

	if (empty_line_index == std::string::npos)
		throw Webserv_exception("Invalid request", ERROR);
	res = str.substr(0, empty_line_index);
	return res;
}

AHTTPMessage::AHTTPMessage(void)
{
}

AHTTPMessage::AHTTPMessage(std::string const &raw):  version("undefined"), headers(0),raw_data(raw)
{
	std::string					raw_meta_data = get_meta_data(raw);
	std::vector<std::string>	meta_data = split(raw_meta_data, "\n");

	body = parse_body(raw);
}

std::string	AHTTPMessage::parse_body(std::string const &raw) const
{
	return (raw.substr(raw.find("\n\n")));
}

AHTTPMessage::~AHTTPMessage(){}

std::ostream	&operator<<(std::ostream &os, AHTTPMessage const &rhs)
{
	os << "" << &rhs;
	return (os);
}

std::string const &AHTTPMessage::get_raw_data(void) const
{
	return (raw_data);
}