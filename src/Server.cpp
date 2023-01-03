#include "../inc/Server.hpp"
#include "Server.hpp"

Server::Server(void): logger(Logger()), connections_number(0)
{
	logger.log("created server with default logger", INFO);
}

Server::Server(Logger const &logger) : logger(logger)
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
			// Query.recieve();
			// Query.send(std::string("Hello, world"));
		}
	}
	catch(const Webserv_exception & e)
	{
		logger.log(e.what(), e.get_error_code());
	}
}

void	Server::answer(void)
{
	Query *p;
	if (queries.empty())
		return;
	for (std::vector<Query *>::iterator it = queries.begin();
		it != queries.end(); it++)
	{
		if (queries.empty())
			break ;
		p = *it;
		(*it)->send("Hello, World");
		logger.log("message sent", INFO);
		queries.erase(it);
		delete p;
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
		}
		catch(const Webserv_exception & e)
		{
			logger.log(e.what(), e.get_error_code());
		}
		
	}
}

void	Server::serve(void)
{
	int i = 0;
	while (i < 5)
	{
		std::cout << i << "\n";
		poll();
		collect();
		answer();
		::sleep(1);
		i++;
	}
}

Server::~Server()
{
	logger.log("Server stopped", INFO);
}

