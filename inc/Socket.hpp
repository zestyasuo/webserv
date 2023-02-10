#ifndef Socket_HPP
#define Socket_HPP
#include "Webserv_exception.hpp"
#include "utils.hpp"
#include <iostream>
#include <netinet/in.h>
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>
#define BACKLOG 50
#define SSTR(x) static_cast< std::ostringstream & >((std::ostringstream() << std::dec << x)).str()

class Socket
{
  private:
	int				   port;
	struct sockaddr_in address;
	int				   sfd;
	int				   opt;
	void			   create(void);
	void			   bind(void);
	void			   listen(void);
	Socket(void);
	Socket(Socket const &copy);
	Socket &operator=(const Socket &);

  public:
	void poll(void);
	int	 get_fd(void) const;
	int	 get_port(void) const;
	Socket(int port);
	~Socket(void);
	class SocketCreationFailedException : public Webserv_exception
	{
	};
	class SocketBindingFailedException : public Webserv_exception
	{
	};

	class SocketListenFailureException : public Webserv_exception
	{
	};
};

#endif