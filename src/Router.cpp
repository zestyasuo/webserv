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

Router::Router(std::vector< s_config > const &conf) : configs(conf), logger(true, "Router")
{
	// open requested sockets

	for (std::vector< s_config >::iterator it = configs.begin(); it != configs.end(); it++)
	{
		for (std::vector< int >::iterator port_it = (*it).ports.begin(); port_it != (*it).ports.end(); port_it++)
		{
			if (!open_sockets.count(*port_it))
			{
				open_sockets.insert(std::make_pair< int, Socket * >(*port_it, new Socket(*port_it)));
				// fds[open_sockets.size() - 1].fd = open_sockets[*port_it]->get_fd();
				// fds[open_sockets.size() - 1].events = POLLIN | POLLOUT;
				logger.log("Serving port " + SSTR(*port_it), DEBUG);
			}
		}
	}
	// create reqested servers
	for (std::vector< s_config >::iterator it = configs.begin(); it != configs.end(); it++)
	{
		for (std::vector< int >::iterator port_it = (*it).ports.begin(); port_it != (*it).ports.end(); port_it++)
		{
			if (!servers.count(open_sockets.at(*port_it)))
				servers.insert(std::make_pair< Socket *, std::vector< Server * > >(open_sockets.at(*port_it),
																				   std::vector< Server * >()));
			servers.at(open_sockets.at(*port_it)).push_back(new Server(logger, *it));
		}
	}
}

Router::~Router()
{
	for (std::map< Socket *, std::vector< Server * > >::iterator it = servers.begin(); it != servers.end(); it++)
	{
		for (std::vector< Server * >::iterator serv_it = (*it).second.begin(); serv_it != (*it).second.end(); serv_it++)
			delete *serv_it;
	}

	for (std::map< int, Socket * >::iterator it = open_sockets.begin(); it != open_sockets.end(); it++)
		delete (*it).second;

	for (std::vector< Query * >::iterator it = queries.begin(); it != queries.end(); it++)
		delete *it;
}

void Router::poll(void)
{

	for (std::map< int, Socket * >::iterator it = open_sockets.begin(); it != open_sockets.end(); it++)
	{
		int fd = ::accept((*it).second->get_fd(), NULL, NULL);
		if (fd < 0)
			continue;
		unblock_fd(fd);
		pollfd tmp = {};
		tmp.fd = fd;
		tmp.events = POLLIN | POLLOUT;
		fds_vec.push_back(tmp);
		queries.push_back(new Query(fds_vec.back().fd));
		logger.log("connection accepted", INFO);
	}
	pollfd *fds = fds_vec.data();
	if (::poll(fds, fds_vec.size(), 0) < 0)
		throw Webserv_exception("poll failied", ERROR);
	for (
		struct {
			size_t							i;
			std::vector< pollfd >::iterator poll_it;
			std::vector<Query *>::iterator query_it;
		} loop = {0, fds_vec.begin(), queries.begin()};
		loop.poll_it != fds_vec.end();)
	{
		logger.log(SSTR(fds_vec.size()) + " ? " + SSTR(queries.size()), FATAL);
		if ((*loop.poll_it).revents == 0)
		{
			loop.poll_it++;
			loop.i++;
			loop.query_it++;
			logger.log("skip befo process", DEBUG);
			continue;
		}
		if (!process(*loop.query_it, *loop.poll_it))
		{
			delete *loop.query_it;
			loop.poll_it = fds_vec.erase(loop.poll_it);
			loop.query_it = queries.erase(loop.query_it);
		}
		else
		{
			logger.log("skip after process", DEBUG);
			loop.query_it++;
			loop.poll_it++;
			loop.i++;
		}
	}
}

ssize_t Router::collect(Query *from)
{
	ssize_t status = from->recieve();
	if (from->is_ready())
		from->form_request();
	return status;
}

void Router::respond(Query *to)
{
	if (to->get_request())
	{
		// std::string host =
		// to->get_request()->get_headers().count("Host") ? (*it)->get_request()->get_headers().at("Host") : "";
		// Socket *from_socket = get_socket_by_fd(to->get_socket());
		// Server *respond_from = find_server_bound_to_socket_by_name(host, from_socket);
		servers.at(open_sockets.at(8090))[0]->respond(to);
	}
}

Server *Router::find_server_bound_to_socket_by_name(std::string const &name, Socket *from_socket)
{
	if (!from_socket)
		return NULL;
	if (!servers.count(from_socket))
		return NULL;

	std::vector< Server * > socket_servers = servers.at(from_socket);
	if (name.empty())
		return socket_servers.front();
	for (std::vector< Server * >::iterator it = socket_servers.begin(); it != socket_servers.end(); it++)
	{
		if ((*it)->get_config().name == name)
			return *it;
	}
	return socket_servers.front();
}

Socket *Router::get_socket_by_fd(int const fd) const
{
	for (std::map< int, Socket * >::const_iterator it = open_sockets.begin(); it != open_sockets.end(); it++)
	{
		if ((*it).second->get_fd() == fd)
			return (*it).second;
	}
	return NULL;
}

bool Router::process(Query *query, pollfd &p)
{
	logger.log("process", INFO);
	if (p.revents & POLLHUP || p.revents & POLLERR)
		return false;
	else if (p.revents & POLLIN)
	{
		logger.log("collecting", INFO);
		if (collect(query) <= 0)
			return false;
		return true;
	}
	else if (p.revents & POLLOUT)
	{
		logger.log("responding", INFO);
		respond(query);
		return false;
	}
	return true;
}

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