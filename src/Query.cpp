#include "../inc/Query.hpp"
# include <stdio.h>
Query::Query()
{
}

Query::Query(struct pollfd sockfd) : poll_fd(sockfd), socket_fd(sockfd.fd), raw_data(""), request(0)
{
	fd = accept(socket_fd, NULL, NULL);
	if (fd < 0)
		throw Webserv_exception("accept failed", FATAL);
	unblock_fd(fd);
}

int		Query::recieve(void)
{
	char	buf[128];
	int	recieved_bytes = 0;
	int	i;
	const int	bytes_to_recieve = sizeof(buf);

	while (recieved_bytes != bytes_to_recieve)
	{
		i = ::recv(fd, buf + recieved_bytes, bytes_to_recieve - recieved_bytes, 0);
		if (i < 0)
			break ;
		if (i == 0)
			break ;
		recieved_bytes += i;
	}
	raw_data += buf;
	return recieved_bytes;
}

int	Query::send(std::string const &message) const
{
	char *bytes = const_cast<char *>(message.c_str());
	int code = ::send(fd, bytes, message.length(), 0);
	if (code < 0)
	{
		perror("sent");
		throw Webserv_exception("send failed", ERROR);
	}
	return message.length();
}

Query::Query(Query const &copy)
{
	std::cout << "Query copy constructor";
	*this = copy;
}

Query::~Query(){close(fd);};

HTTPRequest const	*Query::get_request(void) const
{
	return (request);
}

void	Query::form_request(void)
{
	request = new HTTPRequest(raw_data);
}