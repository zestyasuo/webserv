#include "../inc/HTTPRequest.hpp"

size_t	skip_word(std::string const &str, size_t pos)
{
	size_t i = 0;
	while (!std::isspace(str[pos]) && str[pos] != 0)
	{
		pos++;
		i++;
	}
	return (i);
}

size_t	skip_spaces(std::string const &str, size_t pos)
{
	size_t i = 0;
	while (std::isspace(str[pos]) && str[pos] != 0)
	{
		pos++;
		i++;
	}
	return (i);
}

HTTPRequest::HTTPRequest()
{
}

HTTPRequest::HTTPRequest(std::string const &raw) : AHTTPMessage(raw)
{
	method = parse_method(get_meta_data()[0]);
	(void) version;
	(void) target;
	headers = parse_headers(get_meta_data());
}

HTTPRequest::HTTPRequest(HTTPRequest const &rhs) : AHTTPMessage(rhs)
{
	version = rhs.version;
	body = rhs.body;
	target = rhs.target;
	method = rhs.method;
	headers = rhs.headers;
}

HTTPRequest::~HTTPRequest(){}

std::ostream	&operator<<(std::ostream &os, HTTPRequest const &rhs)
{
	os << "HTPP request:\n" << rhs.get_raw_data() << "\n-----------\n"
	<< "Method" << rhs.get_method() << "\nHeaders:\n";
	for (std::map<std::string, std::string>::const_iterator it = rhs.get_headers().begin();
			it != rhs.get_headers().end(); it++)
	{
		os << (*it).first << " : " << (*it).second << "\n";
	}
	os << "Body: \n" << rhs.get_body() << "\nend";
	return (os);
}

HTTPRequest	&HTTPRequest::operator=(HTTPRequest const &rhs)
{
	if (this != &rhs)
	{
		version = rhs.version;
		body = rhs.body;
		target = rhs.target;
		method = rhs.method;
		headers = rhs.headers;
	}
	return (*this);
}

std::map<std::string, std::string> HTTPRequest::parse_headers(std::vector<std::string> const &meta) const
{
	std::string first;
	std::string second;
	std::map<std::string, std::string> res;

	for (std::vector<std::string>::const_iterator it = meta.begin() + 1; it != meta.end(); it++)
	{
		first = (*it).substr(0, (*it).find(":"));
		second = (*it).substr((*it).find(":"));
		res.insert(std::make_pair(first, second));
	}

	return (res);
}

std::string HTTPRequest::parse_method(std::string const &status_line) const
{
	size_t		i = 0;
	std::string					res = "";

	i += skip_spaces(status_line, i);
	res = status_line.substr(i, i + skip_word(status_line, i));

	return res;
}

// loop? idk. mb better approuch?
std::string HTTPRequest::parse_target(std::string const &status_line) const
{
	size_t	i = 0;
	std::string res = "";

	i += skip_spaces(status_line, i);
	i += skip_word(status_line, i);
	i += skip_spaces(status_line, i);
	res = status_line.substr(i, i + skip_word(status_line, i));
	return (res);
}

// get_n_word_in_str(const string str, int word_number)
// while (i < n)
// 		skip_spaces + skipword
// substr(i+skipword)
std::string	HTTPRequest::parse_version(std::string const &status_line) const
{
	size_t i = 0;
	std::string res = "";

	i += skip_spaces(status_line, i);
	i += skip_word(status_line, i);
	i += skip_spaces(status_line, i);
	i += skip_word(status_line, i);
	i += skip_spaces(status_line, i);
	res = status_line.substr(i, i + skip_word(status_line, i));
	return res;
}

std::string const &HTTPRequest::get_method(void) const
{
	return method;
}

std::string	const &HTTPRequest::get_target(void) const
{
	return (target);
}