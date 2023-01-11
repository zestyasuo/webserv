# include "../inc/utils.hpp"

void	unblock_fd(int sfd)
{
	int	flags = fcntl(sfd, F_GETFL, 0);
	if (flags < 0)
		throw Webserv_exception("Couldn't retrieve socket flags", FATAL);
	flags = flags | O_NONBLOCK;
	if (fcntl(sfd, F_SETFL, flags) < 0)
		throw Webserv_exception("Couldn't set non-block flag", FATAL);

}

std::vector<std::string> split(std::string const &str, std::string const &del)
{
	size_t						pos = 0;
	std::string					token;
	std::string					copy;
	std::vector<std::string>	res;

	copy = str;
	while ((pos = copy.find(del)) != std::string::npos)
	{
		token = copy.substr(0, pos);
		res.push_back(token);
		copy.erase(0, pos + del.length());
	}
	if (copy.find(del) == std::string::npos && copy != "")
		res.push_back(copy);
	
	return res;
}