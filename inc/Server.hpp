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
# include "Config_proto.hpp"
# define CONNECTIONS_COUNT 50
# define TIMEOUT 100

# define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()


class Server
{
private:
	Logger				const logger;
	bool 				active;
	t_conf	 			config;
public:
	void				respond(Query *query);
	Server(void);
	Server(Logger const &, t_conf const &);
	~Server(void);
	Server	&operator=(Server const &rhs);
	t_conf	const &get_config(void) const;
	class TooManyConnectionsException : public Webserv_exception {
	};
};

std::ostream	&operator<<(std::ostream &os, Server const &rhs);
#endif