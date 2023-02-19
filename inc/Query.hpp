#ifndef Query_HPP
#define Query_HPP
#include "../inc/utils.hpp"
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"
#include "Socket.hpp"
#include "Webserv_exception.hpp"
#include <cstddef>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#define BUFF_SIZE 1024

class Query
{
  private:
	Socket		 *socket;
	int			  fd;
	std::string	  raw_data;
	HTTPResponse *response;
	HTTPRequest	 *request;
	Query(void);
	Query(Query const &copy);
	bool	ready;
	ssize_t content_length;

  public:
	Socket			   *get_socket(void) const;
	bool				is_ready(void) const;
	void				form_request(void);
	void				form_response(t_conf const &);
	HTTPRequest const  *get_request(void) const;
	HTTPResponse const *get_response(void) const;
	ssize_t				recv(void);
	ssize_t				recieve(void);
	std::string			recieve_headers(void);
	void				recieve_body(void);
	size_t				send() const;
	Query(Socket *, int);
	~Query(void);
};

#endif
