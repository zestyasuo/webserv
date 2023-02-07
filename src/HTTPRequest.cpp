#include "../inc/HTTPRequest.hpp"

HTTPRequest::HTTPRequest() : valid()
{
}

HTTPRequest::HTTPRequest(std::string const &raw) : AHTTPMessage(raw), valid(false), target(""), method("")
{
	std::vector< std::string > meta_data = get_meta_data();
	if (meta_data.empty())
		return;
	std::string				   status_line = meta_data[0];
	std::vector< std::string > status_line_vec = split_if(status_line, ::isspace);

	if (status_line_vec.size() != 3)
		return;
	method = parse_method(status_line_vec);
	target = parse_target(status_line_vec);
	version = parse_version(status_line_vec);
	headers = parse_headers(meta_data);
	valid = true;
}

HTTPRequest::HTTPRequest(HTTPRequest const &rhs) : AHTTPMessage(rhs), valid(), target(rhs.target), method(rhs.method)
{
	version = rhs.version;
	body = rhs.body;

	headers = rhs.headers;
}

HTTPRequest::~HTTPRequest()
{
}

std::ostream &operator<<(std::ostream &os, HTTPRequest const &rhs)
{
	os << "HTPP request:\n"
	   << rhs.get_raw_data() << "-----------\n"
	   << "Method ---" << rhs.get_method() << "\n"
	   << "Target ---" << rhs.get_target() << "\n"
	   << "Version ---" << rhs.get_version()
	   << "\n"
		  "\nHeaders ---\n";
	for (std::map< std::string, std::string >::const_iterator it = rhs.get_headers().begin(); it != rhs.get_headers().end(); it++)
	{
		os << (*it).first << "---" << (*it).second << "\n";
	}
	os << "Body: \n" << rhs.get_body() << "end.\n";
	return (os);
}

HTTPRequest &HTTPRequest::operator=(HTTPRequest const &rhs)
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

std::map< std::string, std::string > HTTPRequest::parse_headers(std::vector< std::string > const &meta) const
{
	std::string							 first;
	std::string							 second;
	std::map< std::string, std::string > res;

	for (std::vector< std::string >::const_iterator it = meta.begin() + 1; it != meta.end(); it++)
	{
		first = (*it).substr(0, (*it).find(":"));
		second = (*it).substr((*it).find(":") + 1);
		first.erase(std::remove_if(first.begin(), first.end(), ::isspace), first.end());
		second.erase(std::remove_if(second.begin(), second.end(), ::isspace), second.end());
		res.insert(std::make_pair(first, second));
	}

	return (res);
}

std::string HTTPRequest::parse_method(std::vector< std::string > const &status_line) const
{
	return status_line[METHOD];
}

std::string HTTPRequest::parse_target(std::vector< std::string > const &status_line) const
{
	return status_line[TARGET];
}

std::string HTTPRequest::parse_version(std::vector< std::string > const &status_line) const
{
	return status_line[VERSION];
}

std::string const &HTTPRequest::get_method(void) const
{
	return method;
}

std::string const &HTTPRequest::get_target(void) const
{
	return (target);
}

bool HTTPRequest::is_valid(void) const
{
	return valid;
}
