#ifndef HTTPResponse_HPP 
# define HTTPResponse_HPP
# include <iostream>
# include "AHTTPMessage.hpp"
# include "HTTPRequest.hpp"
# include "Config_proto.hpp"
# include <fstream>

class HTTPResponse : public AHTTPMessage
{
private:
	std::string	version;
	int			status_code;
	std::string	status_text;
	std::string	content_type;
	std::string	payload;
	HTTPRequest	const *request;
	std::string								parse_version(std::vector<std::string> const &) const;
	std::map<std::string, std::string>		parse_headers(std::vector<std::string> const &) const;
	void									add_header(std::string const &, std::string const &);
	void									insert_status_line(void);
	t_conf		const config;
	bool		isMethodAllowed(s_location const &);
	void		get_file_info(std::string const &);
	void		read_file(std::ifstream &);
	void		process_target(std::string const &, s_location const &);
	int			try_index_page(std::string const &, s_location const &);
	HTTPResponse	&operator=(HTTPResponse const &rhs);
public:
	HTTPResponse(void);
	explicit HTTPResponse(const HTTPRequest *, t_conf const &);
	HTTPResponse(HTTPResponse const &copy);
	~HTTPResponse(void);
	std::string		to_string();
};

std::ostream	&operator<<(std::ostream &os, HTTPResponse const &rhs);
#endif