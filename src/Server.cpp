#include "../inc/Server.hpp"
#include "log_levels.hpp"
#include <sys/poll.h>

Server::Server(void) : logger(Logger()), active(true), config(), fd()
{
	logger.log("created server with default logger", INFO);
}

Server::Server(Logger const &logger, t_conf const &conf, int fd) : logger(logger), active(true), config(conf), fd(fd)
{
	logger.log("server created", INFO);
}

t_conf const &Server::get_config(void) const
{
	return config;
}

int Server::get_fd(void) const
{
	return fd;
}

bool Server::respond(Query *query)
{
	query->form(config);
	query->send();
	logger.log("message sent from " + config.name, INFO);
	return true;
}

Server::~Server()
{
	logger.log("Server stopped", INFO);
}
