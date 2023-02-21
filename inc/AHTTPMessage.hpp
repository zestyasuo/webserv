#ifndef AHTTPMessage_HPP
#define AHTTPMessage_HPP
#include "utils.hpp"
#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

class AHTTPMessage
{
  protected:
	std::string									 version;
	std::map< std::string, std::string >		 headers;
	std::string									 body;
	virtual std::string							 parse_version(std::vector< std::string > const &) const = 0;
	virtual std::map< std::string, std::string > parse_headers(std::vector< std::string > const &) const = 0;
	std::string									 parse_body(std::string const &) const;
	std::vector< std::string >					 parse_meta_data(std::string const &);
	std::string									 raw_data;
	std::vector< std::string >					 meta_data;

  private:
  public:
	std::string const						   &get_body(void) const;
	std::map< std::string, std::string > const &get_headers(void) const;
	std::string const						   &get_version(void) const;
	std::string const						   &get_raw_data(void) const;
	std::vector< std::string > const		   &get_meta_data(void) const;
	AHTTPMessage(AHTTPMessage const &);
	AHTTPMessage(void);
	AHTTPMessage(std::string const &);
	virtual ~AHTTPMessage(void);
};

#endif