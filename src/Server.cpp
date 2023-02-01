#include "../inc/Server.hpp"

Server::Server(void): logger(Logger()), active(true), config()
{
	logger.log("created server with default logger", INFO);
}

Server::Server(Logger const &logger, t_conf const &conf) : logger(logger),
	active(true), config(conf)
{
	logger.log("server created", INFO);
}

t_conf	const &Server::get_config(void) const
{
	return config;
}

void	Server::respond(Query *query)
{
		if ((query)->get_request())
		{
			HTTPResponse *response = new HTTPResponse(query->get_request(), config);
			query->send(response->to_string());
			logger.log("message sent from " + config.name, INFO);
			delete response;
		}
}

Server::~Server()
{
	// for (std::map<int, Socket *>::iterator it = sockets.begin(); it != sockets.end(); it++)
	// {
	// 	delete (*it).second;
	// }
	logger.log("Server stopped", INFO);
}
