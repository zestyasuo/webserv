#ifndef Router_HPP 
# define Router_HPP

# include <iostream>
# include <poll.h>
# include <vector>
# include "Server.hpp"
# include "Webserv_exception.hpp"
# include "Config_proto.hpp"
# include "log_levels.hpp"
# include "Socket.hpp"
# include "Query.hpp"
# define TIMEOUT 100

class Router
{
private:
	std::vector<s_config> configs; // parsed server configs
	std::vector<Query *> queries;
	std::map<int, Socket *>	open_sockets;
	std::map<Socket *, std::vector<Server *> >	servers;
	struct pollfd fds[1024];
	Router(Router const &copy);
	Router(void);
	Router	&operator=(Router const &rhs);
	void	poll(void);
	void	collect(void);
	void	respond(void);
public:
	void	serve(void);
	Router(std::vector<s_config> const &);
	~Router(void);
};

#endif