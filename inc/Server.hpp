#ifndef Server_HPP
#define Server_HPP
#include "Config.hpp"
#include "HTTPResponse.hpp"
#include "Logger.hpp"
#include "Query.hpp"
#include "Socket.hpp"
#include "Webserv_exception.hpp"
#include "log_levels.hpp"
#include <cstdio>
#include <iostream>
#include <map>
#include <poll.h>
#include <sstream>
#include <utility>
#include <vector>
#define CONNECTIONS_COUNT 256
#define TIMEOUT 1
#define FOREVER -1
#include "html.hpp"
#define SSTR(x) static_cast< std::ostringstream & >((std::ostringstream() << std::dec << x)).str()

class Query;

class Server
{
  private:
	Logger const logger;
	bool		 active;
	t_conf		 config;
	int			 fd;
	Server(Server const &);

  public:
	bool respond(Query *query);
	Server(void);
	Server(Logger const &, t_conf const &, int);
	~Server(void);
	Server		 &operator=(Server const &rhs);
	t_conf const &get_config(void) const;
	int			 get_fd(void) const;
	class TooManyConnectionsException : public Webserv_exception
	{
	};
};

std::ostream &operator<<(std::ostream &os, Server const &rhs);
#endif
