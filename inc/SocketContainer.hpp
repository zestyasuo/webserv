#pragma once
#include "Query.hpp"
#include <cstddef>
#include <sys/poll.h>
#include <sys/types.h>
#include <vector>

class SocketContainer
{
	SocketContainer(SocketContainer const &);
	SocketContainer		  &operator=(SocketContainer const &);
	std::vector< Query * > queries;
	std::vector< pollfd >  fds;
	bool				   all_sockets;
	ssize_t				   socket_count;

  public:
	~SocketContainer();
	SocketContainer();
	void						   accept();
	int							   poll(void);
	void						   push_query(int, int);
	void						   push_listener(int fd);
	size_t						   queries_size();
	std::pair< pollfd , Query * > get_query(size_t i);
	void						   erase_query(size_t i);
};
