#ifndef Query_HPP
#define Query_HPP
#include "../inc/utils.hpp"
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"
#include "Socket.hpp"
#include "Webserv_exception.hpp"
#include "Server.hpp"
#include <cstddef>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#define BUFF_SIZE 1024

class Server;

class Query
{
  private:
	enum em_state
	{
		headers,
		body,
		forming,
		sending,
		done
	};
	em_state	  state;
	int			  fd;
	int			  socket;
	size_t		  pending_bytes;
	t_conf		  config;
	std::vector< Server * > &servers;
	HTTPResponse *response;
	HTTPRequest	 *request;
	Query(void);
	Query(Query const &copy);
	ssize_t content_length;

  public:
	void				respond(void);
	bool				is_done(void) const;
	bool				is_forming(void) const;
	int					get_socket(void) const;
	int					get_fd(void) const;
	bool				is_ready(void) const;
	void				form(t_conf const &);
	HTTPRequest const  *get_request(void) const;
	HTTPResponse const *get_response(void) const;
	void				recv(void);
	ssize_t				recieve(void);
	std::string			recieve_headers(void);
	void				recieve_body(void);
	void				send();
	bool				process(int);
	Query(int, int, std::vector< Server * > &servers);
	~Query(void);
};

#endif
