#include "SocketContainer.hpp"
#include "Query.hpp"
#include "Webserv_exception.hpp"
#include <cstddef>
#include <sys/poll.h>
#include <sys/socket.h>
#include <utility>
#include <vector>

SocketContainer::SocketContainer() : queries(), fds(), all_sockets(false), socket_count(0)
{}

SocketContainer::~SocketContainer ()
{
	for (std::vector<Query *>::iterator it = queries.begin(); it != queries.end(); it++)
		delete *it;
}	

void SocketContainer::push_query(int socket, int fd)
{
	all_sockets = true;
	pollfd tmp = {};
	tmp.fd = fd;
	tmp.events = POLLIN | POLLOUT;
	tmp.revents = POLLIN;
	queries.push_back(new Query(fd, socket));
	fds.push_back(tmp);
}

void	SocketContainer::push_listener(int fd)
{
	if (all_sockets)
		throw Webserv_exception("Do not mix sockets", ERROR);
	pollfd tmp = {};
	tmp.fd = fd;
	tmp.events = POLLIN;
	fds.push_back(tmp);
	socket_count++;
}

int	SocketContainer::poll()
{
	std::clog << "about to poll\n";
	int status = ::poll(fds.data(), fds.size(), -1);
	if (status < 0)
		throw Webserv_exception("poll failied", ERROR);
	return status;
}

size_t	SocketContainer::queries_size()
{
	return queries.size();
}

std::pair<pollfd, Query *>	SocketContainer::get_query(size_t i)
{
	return std::make_pair(fds[i + socket_count], queries[i]);
}

void	SocketContainer::erase_query(size_t i)
{
	fds.erase(fds.begin() + i + socket_count);
	delete queries[i];
	queries.erase(queries.begin() + i);
}

void	SocketContainer::accept()
{
	for (ssize_t i = 0; i < socket_count; i++)
	{
		if (fds.data()[i].revents & POLLIN)
		{
			int fd = ::accept(fds.data()[i].fd, NULL, NULL);
			if (fd < 0)
				throw Webserv_exception("accept failed", ERROR);
			unblock_fd(fd);
			push_query(fd, fds.data()[i].fd);
		}
	}
}