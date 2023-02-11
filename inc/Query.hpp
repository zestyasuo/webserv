#ifndef Query_HPP
#define Query_HPP
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"
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
	HTTPResponse  *response;
	HTTPRequest	  *request;
	Query(void);
	Query(Query const &copy);
	bool ready;

  public:
	int					get_socket(void) const;
	bool				is_ready(void) const;
	void				form_request(void);
	void				form_response(t_conf const &);
	HTTPRequest const  *get_request(void) const;
	HTTPResponse const *get_response(void) const;
	size_t				recieve(void);
	size_t				send() const;
	Query(struct pollfd *);
	short getRevents(void) const;
	~Query(void);
};

#endif
