#ifndef Query_HPP 
# define Query_HPP
# include <iostream>
# include <sys/socket.h>
# include <Webserv_exception.hpp>
# include <unistd.h>
# include "utils.hpp"
# include "HTTPReqest.hpp"
# include "HTTPResponse.hpp"

class Query
{
private:
	int				socket_fd;
	int				fd;
	std::string		raw_data;
	HTTPReqest		request;
	HTTPResponse 	response;
	Query(void);
	Query(Query const &copy);
public:
	int		recieve(void);
	int		send(std::string const &) const;
	Query(int);
	~Query(void);
};

#endif