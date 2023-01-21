#include "../inc/Socket.hpp"
# include <cstdio>

Socket::Socket(int port)
{
	this->port = port;
	opt = 1;
	sfd = -1;
	create();
	bind();
	listen();
}

Socket::Socket(Socket const &copy)
{
	port = copy.port;
	sfd = copy.sfd;
}

Socket::~Socket()
{
	::shutdown(sfd, SHUT_RDWR);
	std::cout << "Socket destroyed\n";
}

int	Socket::get_port(void) const
{
	return (port);
}

int		Socket::get_fd(void) const
{
	return (sfd);
}

void	Socket::create()
{
	sfd = ::socket(AF_INET, SOCK_STREAM, 0);
		if (sfd < 0) {
			throw Webserv_exception("Socket creation failed", FATAL);
		}
	if (
		::setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) &&
		::setsockopt(sfd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt))
		)
	{
		throw Webserv_exception("Setting socket options failed", FATAL);
	}

	unblock_fd(sfd);
}

void	Socket::listen()
{
	if (::listen(sfd, BACKLOG) == -1)
		throw Webserv_exception("Listen Failed", FATAL);

}

void	Socket::bind()
{
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);

	if (::bind(sfd, (struct sockaddr *)&address, sizeof(address)) < 0)
		throw Webserv_exception("Socket binding failed", FATAL);
}
