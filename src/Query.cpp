#include "../inc/Query.hpp"
# include <stdio.h>
Query::Query()
{
	std::cout << "Def Query constructor\n";
}

Query::Query(int sockfd) : socket_fd(sockfd), raw_data("")
{
	fd = accept(socket_fd, NULL, NULL);
	if (fd < 0)
		throw Webserv_exception("accept failed", FATAL);
}

int		Query::recieve(void)
{
	char	buf[1024];
	int	recieved_bytes = recv(fd, buf, sizeof (buf), 0);
	if (recieved_bytes < 0){
		perror("recv");
		throw Webserv_exception("recv failed", ERROR);
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
