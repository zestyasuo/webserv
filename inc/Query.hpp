#ifndef Query_HPP 
# define Query_HPP
# include <iostream>
# include <sys/socket.h>
# include "Webserv_exception.hpp"
# include <unistd.h>
# include "utils.hpp"
# include "HTTPRequest.hpp"
# include "HTTPResponse.hpp"

class Query
{
private:
	struct pollfd				*socket;
	int				fd;
	std::string		raw_data;
	HTTPRequest		*request;
	HTTPResponse 	response;
	Query(void);
	Query(Query const &copy);
	bool		ready;
public:
	bool	sent;
	int			get_socket(void) const;
	bool		is_ready(void) const;
	void		form_request(void);
	HTTPRequest const	*get_request(void) const;
	int		recieve(void);
	int		send(std::string const &) const;
	Query(struct pollfd *);
	short getRevents(void) const;
	~Query(void);
};

#endif
