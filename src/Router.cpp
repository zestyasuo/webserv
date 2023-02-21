#include "../inc/Router.hpp"
#include "Query.hpp"
#include "Server.hpp"
#include "Socket.hpp"
#include "Webserv_exception.hpp"
#include "log_levels.hpp"
#include "utils.hpp"
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <vector>

Router::Router(std::vector< s_config > const &conf) : configs(conf), sockets(servers), logger(true, "Router")
{
	// open requested sockets

	for (std::vector< s_config >::iterator it = configs.begin(); it != configs.end(); it++)
	{
		for (std::vector< int >::iterator port_it = (*it).ports.begin(); port_it != (*it).ports.end(); port_it++)
		{
			if (!listeners.count(*port_it))
			{
				listeners.insert(std::make_pair< int, Socket * >(*port_it, new Socket(*port_it)));
				sockets.push_listener(listeners[*port_it]->get_fd());
				logger.log("Serving port " + SSTR(*port_it), DEBUG);
			}
		}
	}
	// create reqested servers
	for (std::vector< s_config >::iterator it = configs.begin(); it != configs.end(); it++)
	{
		for (std::vector< int >::iterator port_it = (*it).ports.begin(); port_it != (*it).ports.end(); port_it++)
		{
			int fd = listeners.at(*port_it)->get_fd();
			servers[fd].push_back(new Server(logger, *it, fd));
		}
	}
}

Router::~Router()
{
	for (std::map< int, std::vector< Server * > >::iterator it = servers.begin(); it != servers.end(); it++)
	{
		for (std::vector< Server * >::iterator serv_it = (*it).second.begin(); serv_it != (*it).second.end(); serv_it++)
			delete *serv_it;
	}

	for (std::map< int, Socket * >::iterator it = listeners.begin(); it != listeners.end(); it++)
		delete (*it).second;

	// for (std::vector< Query * >::iterator it = queries.begin(); it != queries.end(); it++)
	// delete *it;
}

void Router::poll(void)
{
	sockets.poll();
	sockets.accept();

	for (size_t i = 0; i < sockets.queries_size();)
	{
		std::pair<pollfd, Query *> cur = sockets.get_query(i);
		if (cur.first.revents == 0)
		{
			i++;
			continue;
		}
		logger.log("here", FATAL);
		if (cur.second->process(cur.first.revents))
		{
			sockets.erase_query(i);
			logger.log("closed connection", DEBUG);
		}
		i++;
	}
}

// Server *Router::find_server_bound_to_socket_by_name(std::string const &name, Socket *from_socket)
// {
// 	if (!from_socket)
// 		return NULL;
// 	if (!servers.count(from_socket))
// 		return NULL;

// 	std::vector< Server * > socket_servers = servers.at(from_socket);
// 	if (name.empty())
// 		return socket_servers.front();
// 	for (std::vector< Server * >::iterator it = socket_servers.begin(); it != socket_servers.end(); it++)
// 	{
// 		if ((*it)->get_config().name == name)
// 			return *it;
// 	}
// 	return socket_servers.front();
// }

// Socket *Router::get_socket_by_fd(int const fd) const
// {
// 	for (std::map< int, Socket * >::const_iterator it = listeners.begin(); it != listeners.end(); it++)
// 	{
// 		if ((*it).second->get_fd() == fd)
// 			return (*it).second;
// 	}
// 	return NULL;
// }

// Server *Router::find_responding_server(Query *query)
// {

// 	std::string host = query->get_request()->get_headers().count("Host") ? query->get_request()->get_headers().at("Host") : "";
// 	Socket *from_socket = get_socket_by_fd(query->get_socket());
// 	return find_server_bound_to_socket_by_name(host, from_socket);
// }

// bool Router::process(Query *query, pollfd &p)	 // false -- delete
// {
// 	static Server *respond_from = 0;
// 	if (p.revents & POLLHUP || p.revents & POLLERR)
// 		return false;
// 	if (p.revents & POLLIN && !query->is_forming())
// 	{
// 		logger.log("collecting", INFO);
// 		query->recieve();
// 		if (!respond_from)
// 		{
// 			logger.log("looking for server", INFO);
// 			respond_from = find_responding_server(query);
// 		}
// 		std::clog << respond_from << "\n";
// 		return true;
// 	}
// 	if (respond_from)
// 	{
// 		logger.log("forming", INFO);
// 		query->form(respond_from->get_config());
// 	}
// 	if (p.revents & POLLOUT && query->is_ready())
// 	{
// 		logger.log("responding", INFO);
// 		query->send();
// 		// respond(query);
// 		if (query->is_done())
// 		{
// 			respond_from = 0;
// 			return false;
// 		}
// 		return true;
// 	}
// 	return false;
// }

void Router::serve(void)
{
	try
	{
		poll();
	}
	catch (Webserv_exception &e)
	{
		logger.log(e.what(), e.get_error_code());
	}
}