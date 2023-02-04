#include "../inc/utils.hpp"

void unblock_fd(int sfd)
{
	if (fcntl(sfd, F_SETFL, O_NONBLOCK) < 0)
		throw Webserv_exception("Couldn't set non-block flag", FATAL);
}

std::string get_floctime(void)
{
	std::string res = "";
	std::time_t now = std::time(NULL);
	char		buf2[32];

	// std::strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S %Z",
	// std::localtime(&now)); // MSK
	std::strftime(buf2, sizeof(buf2), "%a, %d %b %Y %H:%M:%S %Z",
				  std::gmtime(&now));	 // GMT
	res += buf2;
	return res;
}

std::vector< std::string > split(std::string const &str, std::string const &del)
{
	size_t					   pos = 0;
	std::string				   token;
	std::string				   copy;
	std::vector< std::string > res;

	copy = str;
	while ((pos = copy.find(del)) != std::string::npos)
	{
		token = copy.substr(0, pos);
		if (token.length() == 0)
		{
			copy.erase(pos, 1);
			continue;
		}
		res.push_back(token);
		copy.erase(0, pos + del.length());
	}
	if (copy.find(del) == std::string::npos && copy != "")
		res.push_back(copy);

	return res;
}
