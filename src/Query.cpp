#include "../inc/Query.hpp"
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"
#include "Webserv_exception.hpp"
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <poll.h>
#include <string>
#include <sys/poll.h>
#include <vector>

Query::Query() : socket(), fd(), response(), request(), ready()
{
}

Query::Query(struct pollfd *p)
	: socket(p), fd(accept(socket->fd, NULL, NULL)), raw_data(""), response(0), request(0), ready(false)
{
	if (fd < 0)
		throw Webserv_exception("accept failed", FATAL);
	unblock_fd(fd);
}

size_t Query::recieve(void)
{
	char	  buf[BUFF_SIZE] = {0};
	size_t	  recieved_bytes = 0;
	ssize_t	  i = 0;
	const int bytes_to_recieve = sizeof(buf) - 1;

	// std::cout << "read\n";
	while (recieved_bytes != bytes_to_recieve)
	{
		i = ::recv(fd, buf + recieved_bytes, bytes_to_recieve - recieved_bytes, 0);
		if (i <= 0)
		{
			// socket->events = POLLOUT;
			ready = true;
			break;
		}
		recieved_bytes += i;
	}
	raw_data += std::string(buf);
	socket->revents = 0;
	return recieved_bytes;
}

bool Query::is_ready(void) const
{
	return ready;
}

int Query::get_socket(void) const
{
	return socket->fd;
}

// int	Query::send(std::string const &message) const
size_t Query::send() const
{
	size_t	i = 0;
	size_t buf_size = BUFF_SIZE;
	std::string resp = response->to_string();
	const char *buf = resp.c_str();
	size_t to_send = response->to_string().size();
	while (i < to_send)
	{
		const ssize_t l = ::send(fd, buf + i, std::min(buf_size, to_send - i), 0);
		// std::cout << std::min(buf_size, to_send - i) << "  left\n";
		if (l < 0)
			throw Webserv_exception("send failed", ERROR);
		i += l;
	}
	return i;
}

Query::Query(Query const &copy)
	: socket(copy.socket), fd(copy.fd), raw_data(copy.raw_data), response(copy.response), request(copy.request),
	  ready(copy.ready)
{
}

Query::~Query()
{
	delete request;
	delete response;
	close(fd);
};

HTTPRequest const *Query::get_request(void) const
{
	return (request);
}

HTTPResponse const *Query::get_response(void) const
{
	return (response);
}

void Query::form_response(t_conf const &config)
{
	if (request && !response)
		response = new HTTPResponse(request, config);
}

void Query::form_request(void)
{
	ready = raw_data.empty();
	if (!raw_data.empty())
		request = new HTTPRequest(raw_data);
}

short Query::getRevents(void) const
{
	return socket->revents;
}
