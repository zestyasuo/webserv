#ifndef Query_HPP
#define Query_HPP
#include "HTTPRequest.hpp"
#include "Webserv_exception.hpp"
#include <cstddef>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#define BUFF_SIZE 1024

class Query
{
  private:
	struct pollfd *socket;
	int			   fd;
	std::string	   raw_data;
	HTTPRequest	  *request;
	Query(void);
	Query(Query const &copy);
	bool ready;

  public:
	int				   get_socket(void) const;
	bool			   is_ready(void) const;
	void			   form_request(void);
	HTTPRequest const *get_request(void) const;
	size_t			   recieve(void);
	size_t			   send(std::string const &) const;
	Query(struct pollfd *);
	short getRevents(void) const;
	~Query(void);
};

#endif
