#include "../inc/Query.hpp"
#include <vector>
# include <poll.h>

Query::Query()
{
}

Query::Query(struct pollfd *p) : socket(p), raw_data(""), request(0), ready(false), sent(false)
{
	fd = accept(socket->fd, NULL, NULL);
	if (fd < 0)
		throw Webserv_exception("accept failed", FATAL);
	unblock_fd(fd);
}

int		Query::recieve(void)
{
	char	buf[65536] = {0};
	int	recieved_bytes = 0;
	int	i;
	const int	bytes_to_recieve = sizeof(buf) - 1;

	while (recieved_bytes != bytes_to_recieve)
	{
		i = ::recv(fd, buf + recieved_bytes, bytes_to_recieve - recieved_bytes, 0);
		if (i <= 0)
		{
			ready = true;
			std::cout << "connection closed or recv failed\n";
			break ;
		}
		recieved_bytes += i;
	}
	buf[recieved_bytes] = '\0';
	raw_data += buf;
	socket->revents = 0;
	return recieved_bytes;
}

bool	Query::is_ready(void) const
{
	return ready;
}

int	Query::get_socket(void) const
{
	return socket->fd;
}

//int	Query::send(std::string const &message) const
int	Query::send(std::string const &message) const
{
	char *bytes = const_cast<char *>(message.c_str());
	int  code = 0;
	// if ((socket->revents & POLLOUT) == POLLOUT)
	code = ::send(fd, bytes, message.length(), 0);
	if (code < 0)
	{
		perror("sent");
		throw Webserv_exception("send failed", ERROR);
	}
	return message.length();
}

Query::Query(Query const &copy):
	socket(copy.socket),
	fd(copy.fd),
	raw_data(copy.raw_data),
	request(copy.request),
	response(copy.response),
	ready(copy.ready)
{
}

Query::~Query()
{
	delete request;
	close(fd);
};

HTTPRequest const	*Query::get_request(void) const
{
	return (request);
}

void	Query::form_request(void)
{
	ready = raw_data.empty();
	if (!raw_data.empty())
		request = new HTTPRequest(raw_data);
}

short Query::getRevents(void) const
{
	return socket->revents;
}
