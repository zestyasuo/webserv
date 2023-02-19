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
	query->form_response(config);
	query->send();
	logger.log("message sent from " + config.name, INFO);
	return true;
}

Server::~Server()
{
	logger.log("Server stopped", INFO);
}
