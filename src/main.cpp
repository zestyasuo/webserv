#include <Server.hpp>
#include <Logger.hpp>


int	main(void) 
{
	// Logger	server_logger;
	// Server	webserv(server_logger);

	// webserv.add_socket(8080);
	// webserv.serve();

	// std::cout << webserv.sockets.at(7070);
	std::vector<std::string> strs;
	std::string str = "123 \n 123 ";

	strs = split(str, "\n");

	for (std::vector<std::string>::iterator it = strs.begin(); it != strs.end(); it++)
	{
		std::cout << *it;
	}


}