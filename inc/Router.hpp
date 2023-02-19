#ifndef Router_HPP
#define Router_HPP

#include "Config.hpp"
#include "Query.hpp"
#include "Server.hpp"
#include "Socket.hpp"
#include "Webserv_exception.hpp"
#include "log_levels.hpp"
#include <iostream>
#include <poll.h>
#include <utility>
#include <vector>

class Router
{
  private:
	std::vector< s_config >						  configs;	  // parsed server configs
	std::vector< Query * >						  queries;
	std::map< int, Socket * >					  open_sockets;
	std::map< Socket *, std::vector< Server * > > servers;
	std::vector< pollfd >						  fds_vec;
	Logger										  logger;
	ssize_t									socket_count;
	Router(Router const &copy);
	Router(void);
	Router &operator=(Router const &rhs);
	void	poll(void);
	ssize_t collect(Query *);
	void	respond(Query *);
	bool	process(Query *, pollfd &);
	// utils
	Socket *get_socket_by_fd(int const) const;
	Server *find_server_bound_to_socket_by_name(std::string const &, Socket *);

  public:
	void serve(void);
	Router(std::vector< s_config > const &);
	~Router(void);
};

#endif
