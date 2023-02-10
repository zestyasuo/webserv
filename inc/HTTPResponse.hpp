#ifndef HTTPResponse_HPP
#define HTTPResponse_HPP
#include "AHTTPMessage.hpp"
#include "Config.hpp"
#include "HTTPRequest.hpp"
#include "utils.hpp"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sys/stat.h>	 //	S_IFDIR
#include <sys/wait.h>	 //	S_IFDIR
#include <vector>

#define CTYPE_TEXT_HTML "text/html"

class HTTPResponse : public AHTTPMessage
{
	typedef std::map< int, std::string > int_to_string_map_t;

  private:
	std::string							 version;
	int									 status_code;
	std::string							 status_text;
	std::string							 content_type;
	std::string							 payload;
	std::string							 request_full_path;
	std::string							 request_file_ext;
	std::string							 cgi_query_str;

	HTTPRequest const					*request;
	t_conf const						 config;
	int_to_string_map_t					 error_pages;
	int_to_string_map_t					 status_texts;
	std::string							 parse_version(std::vector< std::string > const &) const;
	std::map< std::string, std::string > parse_headers(std::vector< std::string > const &) const;
	void								 add_header(std::string const &, std::string const &);
	void								 insert_status_line(void);
	bool								 isMethodAllowed(s_location const &);
	void								 get_file_info(std::string const &);
	void								 read_file(std::ifstream &);
	void								 create_file_and_write_contents(std::string const &, std::string const &);
	std::string							 parse_target(std::string const &, std::string const &);
	void								 process_target(std::string const &, s_location const &);
	int									 try_index_page(std::string const &, s_location const &);
	HTTPResponse						&operator=(HTTPResponse const &rhs);
	int									 check_method(s_location const &);
	void								 ready_up(void);
	void								 delete_file(std::string const &);
	bool								is_cgi();
	std::vector< char >					cgi_exec();

	HTTPResponse(void);

  public:
	explicit HTTPResponse(const HTTPRequest *, t_conf const &);
	HTTPResponse(HTTPResponse const &copy);
	~HTTPResponse(void);
	std::string to_string(void) const;
};

std::ostream &operator<<(std::ostream &os, HTTPResponse const &rhs);
#endif