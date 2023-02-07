#ifndef Server_HPP
#define Server_HPP
#include "Config_proto.hpp"
#include "Logger.hpp"
#include "Query.hpp"
#include "Socket.hpp"
#include "Webserv_exception.hpp"
#include "HTTPResponse.hpp"
#include "log_levels.hpp"
#include <cstdio>
#include <iostream>
#include <map>
#include <poll.h>
#include <sstream>
#include <utility>
#include <vector>
#define CONNECTIONS_COUNT 50
#define TIMEOUT 5

#define SSTR(x) static_cast< std::ostringstream & >((std::ostringstream() << std::dec << x)).str()

class Server
{
  private:
	Logger const logger;
	bool		 active;
	t_conf		 config;
	Server(Server const &);
  public:
	void respond(Query *query);
	Server(void);
	Server(Logger const &, t_conf const &);
	~Server(void);
	Server		 &operator=(Server const &rhs);
	t_conf const &get_config(void) const;
	class TooManyConnectionsException : public Webserv_exception
	{
	};
};

std::ostream &operator<<(std::ostream &os, Server const &rhs);
#endif
