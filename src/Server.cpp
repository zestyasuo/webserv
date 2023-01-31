#include "../inc/Server.hpp"
#include "Server.hpp"

Server::Server(void): logger(Logger()), connections_number(0), active(true), config()
{
	logger.log("created server with default logger", INFO);
}

Server::Server(Logger const &logger, t_conf const &conf) : logger(logger), connections_number(0),
	active(true), config(conf)
{
	logger.log("server created", INFO);
}

int		Server::add_socket(int port) {
	try
	{
		if (connections_number > CONNECTIONS_COUNT)
			throw Webserv_exception("Too many connections", ERROR);

		Socket	*sock = new Socket(port);
		if (sockets.insert(std::pair<int, Socket *>(port, sock)).second)
		{
			connections[connections_number].events = POLLIN;
			connections[connections_number].fd = sock->get_fd();
			logger.log("Socket added on port " + SSTR (port) , INFO);
			connections_number++;
		}
	}
	catch(const Webserv_exception & e)
	{
		logger.log(e.what(), e.get_error_code());
		return (1);
	}
	return (0);
}

int	Server::add_socket(std::pair<int, Socket *> sock_pair)
{
	try
	{
		if (connections_number > CONNECTIONS_COUNT)
			throw Webserv_exception("Too many connections", ERROR);
		if (sockets.count(sock_pair.first))
			return 1;
		sockets.insert(sock_pair);
		connections[connections_number].events = POLLIN;
		connections[connections_number].fd = sock_pair.second->get_fd();
		logger.log("Socket added on port " + SSTR (sock_pair.first) , INFO);
		connections_number++;
	}
	catch (const Webserv_exception &e)
	{
		logger.log(e.what(), e.get_error_code());
		return 1;
	}
	return 0;
}


t_conf	const &Server::get_config(void) const
{
	return config;
}

void	Server::poll(void)
{
	try
	{
		if (::poll(connections, connections_number, TIMEOUT) < 0)
			throw Webserv_exception("Poll failed", ERROR);
		for (int i = 0; i < connections_number; i++)
		{
			if (connections[i].revents == 0)
				continue ;
			Query *query = new Query(connections[i].fd);

			queries.push_back(query);
			logger.log("query created", INFO);
		}
	}
	catch(const Webserv_exception & e)
	{
		logger.log(e.what(), e.get_error_code());
	}
}

void	Server::respond(void)
{
	if (queries.empty())
		return;
	for (std::vector<Query *>::iterator it = queries.begin();
		it != queries.end(); it++)
	{
		if (queries.empty())
			break ;
		if ((*it)->get_request())
		{
			HTTPResponse *response = new HTTPResponse((*it)->get_request(), config);
			(*it)->send(response->to_string());
			delete response;
		}
		if ((*it)->is_ready())
		{
			delete (*it);
			queries.erase(it);
		}
	}
}

void	Server::collect(void)
{
	for (std::vector<Query *>::iterator it = queries.begin();
			it != queries.end(); it++)
	{
		try
		{
			(*it)->recieve();
			if ((*it)->is_ready())
			{
				(*it)->form_request();
//				std::cout << "formed\n\n";
//				std::cout << (*(*it)->get_request());
			}
		}
		catch(const Webserv_exception & e)
		{
			delete *it;
			queries.erase(it);
			it--;
			logger.log(e.what(), e.get_error_code());
		}
	}
}

void	Server::serve(void)
{
	if (this->active)
	{
		poll();
		collect();
		respond();
		::usleep(100000);	//	prevents CPU overload in a loop
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
