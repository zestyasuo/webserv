#ifndef HTTPReqest_HPP
#define HTTPReqest_HPP
#include "AHTTPMessage.hpp"
#include <iostream>
#define METHOD 0
#define TARGET 1
#define VERSION 2

class HTTPRequest : public AHTTPMessage
{
  private:
	bool								 valid;
	std::string							 target;
	std::string							 method;
	size_t								 content_length;
	std::map< std::string, std::string > parse_headers(std::vector< std::string > const &) const;
	std::string							 parse_version(std::vector< std::string > const &) const;
	std::string							 parse_method(std::vector< std::string > const &) const;
	std::string							 parse_target(std::vector< std::string > const &) const;
	void								 validate(void);

  public:
	bool			   is_valid(void) const;
	std::string const &get_target() const;
	std::string const &get_method() const;
	HTTPRequest(void);
	HTTPRequest(std::string const &raw);
	HTTPRequest(HTTPRequest const &);
	~HTTPRequest(void);
	HTTPRequest &operator=(HTTPRequest const &rhs);
};

std::ostream &operator<<(std::ostream &os, HTTPRequest const &rhs);
#endif