#include "../inc/Query.hpp"
#include <vector>

Query::Query()
{
}

Query::Query(int sockfd) : socket_fd(sockfd), raw_data(""), request(0), ready(false)
{
	fd = accept(socket_fd, NULL, NULL);
	if (fd < 0)
		throw Webserv_exception("accept failed", FATAL);
	unblock_fd(fd);
}

int		Query::recieve(void)
{
	char	buf[1024];
	int	recieved_bytes = 0;
	int	i;
	const int	bytes_to_recieve = sizeof(buf) - 1;

	while (recieved_bytes != bytes_to_recieve)
	{
		i = ::recv(fd, buf + recieved_bytes, bytes_to_recieve - recieved_bytes, 0);
		if (i <= 0)
		{
			ready = true;
			break ;
		}
		recieved_bytes += i;
	}
	buf[recieved_bytes] = '\0';
	raw_data += buf;
	return recieved_bytes;
}

bool	Query::is_ready(void) const
{
	return ready;
}

//int	Query::send(std::string const &message) const
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
	*this = copy;
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

void		Query::setResponse(HTTPResponse* resp)
{
	this->response = *resp;
}

void	Query::form_request(void)
{
	if (!raw_data.empty())
		request = new HTTPRequest(raw_data);
}