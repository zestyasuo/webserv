#include "../inc/Server.hpp"
#include "log_levels.hpp"
#include <sys/poll.h>

Server::Server(void) : logger(Logger()), active(true), config()
{
	logger.log("created server with default logger", INFO);
}

Server::Server(Logger const &logger, t_conf const &conf) : logger(logger), active(true), config(conf)
{
	logger.log("server created", INFO);
}

t_conf const &Server::get_config(void) const
{
	return config;
}

bool Server::respond(Query *query)
{
	if ((query)->get_request())
	{
		// if (query->getRevents() & POLLOUT)
		// {
			HTTPResponse *response = new HTTPResponse(query->get_request(), config);
			query->send(response->to_string());
			logger.log("message sent from " + config.name, INFO);
			delete response;
			return true;
		// }
		// else
		// {
		// 	logger.log("eeeeeeeeeeeeeeeh", DEBUG);
		// 	return false;
		// }
		// logger.log("message : \n" + response->to_string(), DEBUG);
	}
	return false;
}

Server::~Server()
{
	logger.log("Server stopped", INFO);
}
