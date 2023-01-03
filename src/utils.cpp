# include "../inc/utils.hpp"

void	unblock_fd(int sfd)
{
	int	flags = fcntl(sfd, F_GETFL, 0);
	if (flags < 0)
		throw Webserv_exception("Couldn't retrieve socket flags", FATAL);
	flags = flags | O_NONBLOCK;
	if (fcntl(sfd, F_SETFL, flags) < 0) {
		throw Webserv_exception("Couldn't set non-block flag", FATAL);
	}
}