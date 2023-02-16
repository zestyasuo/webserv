#include "../inc/utils.hpp"
#include <utility>

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

std::string					get_status_message_by_code(int const code)
{

	static std::map< int, std::string > statuses;

	if (statuses.empty()){
		statuses.insert(std::make_pair(200, "OK"));
		statuses.insert(std::make_pair(501, "Not Implemented"));
		statuses.insert(std::make_pair(404, "Not Found"));
		statuses.insert(std::make_pair(405, "Method Not Allowed"));
		statuses.insert(std::make_pair(500, "Internal Server Error"));
		statuses.insert(std::make_pair(301, "Moved Permanently"));
		statuses.insert(std::make_pair(400, "Bad Request"));
	}

	if (statuses.count(code))
		return statuses.at(code);
	return "Undefined error";
}

/// @brief Creates HTTP message compatable string with headers
/// @param m - map of headers
/// @return http message compatable headers
std::string map_to_str(std::map< std::string, std::string > const &m)
{
	std::string res;
	for (std::map< std::string, std::string >::const_iterator it = m.begin(); it != m.end(); it++)
	{
		res += (*it).first + ": " + (*it).second + LB;
	}
	return res;
}