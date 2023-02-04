#include "../inc/AHTTPMessage.hpp"

std::string get_meta_data_array(std::string const &str)
{
	size_t		empty_line_index = str.find(LB LB);
	std::string res = "";

	if (empty_line_index == std::string::npos)
	{
		res = str;
		return res;
	}
	res = str.substr(0, empty_line_index);
	return res;
}

AHTTPMessage::AHTTPMessage(AHTTPMessage const &rhs)
	: raw_data(rhs.raw_data), meta_data(rhs.meta_data)
{
}

AHTTPMessage::AHTTPMessage()
{
}

AHTTPMessage::AHTTPMessage(std::string const &raw)
	: version("undefined"), headers(), raw_data(raw)
{
	std::string raw_meta_data = get_meta_data_array(raw);
	meta_data = split(raw_meta_data, LB);

	body = parse_body(raw);
}

std::string AHTTPMessage::parse_body(std::string const &raw) const
{
	size_t body_pos = raw.find(LB LB);
	if (body_pos == std::string::npos)
		return "";
	std::string res = raw.substr(body_pos + 4);
	return (res);
}

AHTTPMessage::~AHTTPMessage()
{
}

std::ostream &operator<<(std::ostream &os, AHTTPMessage const &rhs)
{
	os << "" << &rhs;
	return (os);
}

std::string const &AHTTPMessage::get_raw_data(void) const
{
	return (raw_data);
}

std::vector< std::string > const &AHTTPMessage::get_meta_data(void) const
{
	return (meta_data);
}

std::string const &AHTTPMessage::get_body(void) const
{
	return (body);
}

std::map< std::string, std::string > const &
AHTTPMessage::get_headers(void) const
{
	return (headers);
}

std::string const &AHTTPMessage::get_version(void) const
{
	return (version);
}