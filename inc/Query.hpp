#ifndef Query_HPP 
# define Query_HPP
# include <iostream>
# include <sys/socket.h>
# include <Webserv_exception.hpp>
# include <unistd.h>
# include "utils.hpp"
# include "HTTPRequest.hpp"
# include "HTTPResponse.hpp"

class Query
{
private:
	int				socket_fd;
	int				fd;
	std::string		raw_data;
	HTTPRequest		*request;
	HTTPResponse 	response;
	Query(void);
	Query(Query const &copy);
public:
	void		form_request(void);
	HTTPRequest const	*get_request(void) const;
	int		recieve(void);
	int		send(std::string const &) const;
	Query(int);
	~Query(void);
};

#endif