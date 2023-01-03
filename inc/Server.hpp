#ifndef Server_HPP 
# define Server_HPP
# include "Webserv_exception.hpp"
# include <vector>
# include <cstdio>
# include <utility>
# include <poll.h>
# include <map>
# include <iostream>
# include "Logger.hpp"
# include <Query.hpp>
# include "log_levels.hpp"
# include "Socket.hpp"
# include <sstream>
# define CONNECTIONS_COUNT 50
# define TIMEOUT 100

# define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()


class Server
{
private:
	Logger				const logger;
	int					connections_number;
	struct pollfd		connections[CONNECTIONS_COUNT];
	// std::vector<Query &>	Querys;

	void				poll();
public:
	std::map<int, Socket &>	sockets;
	int		add_socket(int port);
	Server(void);
	Server(Logger const &logger);
	~Server(void);
	Server	&operator=(Server const &rhs);
	void	serve(void);
	class TooManyConnectionsException : public Webserv_exception {
	};
};

std::ostream	&operator<<(std::ostream &os, Server const &rhs);
#endif