#include "../inc/HTTPRequest.hpp"
#include "Query.hpp"
#include <cstddef>
#include <cstdlib>

HTTPRequest::HTTPRequest() : valid(), content_length(-1)
{
}

HTTPRequest::HTTPRequest(std::string const &raw)
	: AHTTPMessage(raw), valid(false), target(""), method(""), content_length(-1)
{
}

void HTTPRequest::append_raw_data(char const *buf, size_t len)
{
	raw_data.append(buf, len);
}

void HTTPRequest::set_meta_data(std::string const &str)
{
	std::clog << "HERE : \n " <<  str << '\n';
	meta_data = parse_meta_data(str);
}

void HTTPRequest::form()
{
	std::vector< std::string > meta_data = this->parse_meta_data(get_raw_data());
	if (meta_data.empty())
		return;
	std::string				   status_line = meta_data[0];
	std::vector< std::string > status_line_vec = split_if(status_line, ::isspace);
	body = parse_body(get_raw_data());

	if (status_line_vec.size() != 3)
		return;
	method = parse_method(status_line_vec);
	target = parse_target(status_line_vec);
	version = parse_version(status_line_vec);
	headers = parse_headers(meta_data);
	if (headers.count("Content-Length"))
		content_length = std::atoi(headers.at("Content-Length").c_str());
	validate();
}

void HTTPRequest::validate(void)
{
	if (raw_data.empty() || meta_data.empty())
		return;
	if (content_length > 0 && size_t(content_length) != body.length())
		return;
	valid = true;
}

HTTPRequest::HTTPRequest(HTTPRequest const &rhs)
	: AHTTPMessage(rhs), valid(), target(rhs.target), method(rhs.method), content_length(rhs.content_length)
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
	for (std::map< std::string, std::string >::const_iterator it = rhs.get_headers().begin();
		 it != rhs.get_headers().end(); it++)
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

ssize_t HTTPRequest::get_content_length() const
{
	return content_length;
}

std::string const &HTTPRequest::get_target(void) const
{
	return (target);
}

bool HTTPRequest::is_valid(void) const
{
	return valid;
}
