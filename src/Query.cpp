#include "../inc/Query.hpp"
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"
#include "Logger.hpp"
#include "Webserv_exception.hpp"
#include "utils.hpp"
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <poll.h>
#include <string>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <vector>

Query::Query() : state(headers), fd(), socket(), to_send(0), response(0), request(new HTTPRequest()), content_length()
{
}

Query::Query(int f, int s)
	: state(headers), fd(f), socket(s), to_send(0), response(0), request(new HTTPRequest()), content_length(-1)
{
}

ssize_t Query::recieve()
{
	if (state == headers)
	{
		request->set_meta_data(recieve_headers());
		request->parse_headers(request->get_meta_data());
		state = body;
		return request->get_content_length();
	}
	if (state == body)
		return recv();

	return -1;
}

std::string Query::recieve_headers(void)
{
	while (request->get_raw_data().find(LB LB) == std::string::npos)
	{
		std::cout << (state == forming) << "\n";
		std::clog << "geting headers\n";
		recv();
		if (state == forming)
			break;
	}
	return (request->get_raw_data().substr(0, request->get_raw_data().find(LB LB) + 4));
}

bool Query::is_ready(void) const
{
	return state == ready;
}

bool Query::is_forming(void) const
{
	return state == forming;
}

bool Query::is_done(void) const
{
	return state == done;
}

int Query::get_socket(void) const
{
	return socket;
}

int Query::get_fd(void) const
{
	return fd;
}

ssize_t Query::recv(void)
{
	char	  buf[BUFF_SIZE] = {0};
	size_t	  recieved_bytes = 0;
	ssize_t	  i = 0;
	const int bytes_to_recieve = sizeof(buf) - 1;

	while (recieved_bytes != bytes_to_recieve)
	{
		i = ::recv(fd, buf + recieved_bytes, bytes_to_recieve - recieved_bytes, 0);
		if (i <= 0)
		{
			state = forming;
			break;
		}
		recieved_bytes += i;
	}
	request->append_raw_data(buf, recieved_bytes);
	return recieved_bytes;
}

size_t Query::send()
{
	if (!to_send)
		to_send = response->to_string().length();
	char	buf[BUFF_SIZE] = {0};
	size_t	sent_bytes = 0;
	ssize_t i = 0;
	size_t	bytes_to_send = sizeof(buf);
	std::clog << to_send << "\n";
	std::clog << response->to_string().length() << "\n";
	response->to_string().copy(buf, std::min(size_t(BUFF_SIZE - 1), to_send), response->to_string().length() - to_send);
	i = ::send(fd, buf + sent_bytes, std::min(bytes_to_send - sent_bytes, to_send), MSG_NOSIGNAL);
	std::clog << i << "\n";
	if (i < 0)
		throw Webserv_exception("send failed", ERROR);
	sent_bytes += i;
	to_send -= sent_bytes;
	if (to_send == 0)
		state = done;
	return sent_bytes;
}

Query::Query(Query const &copy)
	: state(copy.state), fd(copy.fd), socket(copy.socket), response(copy.response), request(copy.request),
	  content_length(copy.content_length)
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

void Query::form(const t_conf &config)
{
	if (state == forming)
		request->form();
	if (!response)
		response = new HTTPResponse(request, config);
	state = ready;
}

// void Query::respond()
// {

// }
