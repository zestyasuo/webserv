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

Query::Query(int f, int s, std::vector< Server * > &servers)
	: state(headers), fd(f), socket(s), pending_bytes(0), servers(servers), response(0), request(new HTTPRequest()), content_length(-1)
{
}

// bool Query::is_ready(void) const
// {
// 	return state == sending;
// }

// bool Query::is_forming(void) const
// {
// 	return state == forming;
// }

// bool Query::is_done(void) const
// {
// 	return state == done;
// }

int Query::get_socket(void) const
{
	return socket;
}

int Query::get_fd(void) const
{
	return fd;
}

void Query::recv(void)
{
	char	  buf[BUFF_SIZE] = {0};
	ssize_t	  rbytes = 0;

	if (state == headers)
	{
		while (request->get_raw_data().find(LB LB) == std::string::npos)
		{
			rbytes = ::recv(fd, buf, BUFF_SIZE, MSG_NOSIGNAL);
			if (rbytes <= 0)
				return;
			request->append_raw_data(buf, rbytes);
		}

		const std::string &head = request->get_raw_data().substr(0, request->get_raw_data().find(LB LB) + 4);
		request->set_meta_data(head);
		request->parse_headers(request->get_meta_data());
		std::string host = request->get_headers().count("Host") ? request->get_headers().at("Host") : "";
		config = (*servers.begin())->get_config();
		for (std::vector< Server * >::iterator it = servers.begin(); it != servers.end(); it++)
		{
			if ((*it)->get_config().name == host)
			{
				config = (*it)->get_config();
				break;
			}
		}
		ssize_t body_len = request->get_content_length();
		if (body_len > 0)
		{
			pending_bytes = (size_t)body_len;
			state = body;
		} else {
			state = forming;
		}
	}
	if (state == body)
	{
		while (pending_bytes > 0)
		{
			rbytes = ::recv(fd, buf, std::min((size_t)BUFF_SIZE, pending_bytes), MSG_NOSIGNAL);
			if (rbytes <= 0)
				return;
			request->append_raw_data(buf, rbytes);
			pending_bytes -= rbytes;
		}
		state = forming;
	}
}

void Query::send()
{
	ssize_t	sbytes = 0;
	ssize_t	len = response->to_string().length();

	while (pending_bytes > 0)
	{
		sbytes = ::send(fd, response->to_string().c_str() + len - pending_bytes, pending_bytes, MSG_NOSIGNAL);
		if (sbytes <= 0)
			return;
		pending_bytes -= sbytes;
	}
	state = done;
}

Query::Query(Query const &copy)
	: state(copy.state), fd(copy.fd), socket(copy.socket), servers(copy.servers), response(copy.response), request(copy.request),
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
	request->form();
	response = new HTTPResponse(request, config);
	pending_bytes = response->to_string().length();
	state = sending;
}

bool Query::process(int revents)
{
	if (revents & (POLLIN | POLLERR))
		return true;

	if ((state == headers || state == body) && (revents & POLLIN))
		recv();
	else if (state == forming)
		form(config);
	else if (state == sending && (revents & POLLOUT))
		send();
	
	if (state == done)
		return true;
	return false;
}
