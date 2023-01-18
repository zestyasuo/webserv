#include <Server.hpp>
#include <Logger.hpp>
# include <exception>




int	main(void) 
{
	Logger	server_logger;
	Server	*webserv = new Server(server_logger, "/home/zyasuo/21school/my_server/www/serv_a");
	std::vector<Server *>	active_servers;

	webserv->add_socket(8080);
	active_servers.push_back(webserv);
	while (true){
		for (std::vector<Server *>::iterator it = active_servers.begin(); it != active_servers.end(); it++)
			(*it)->serve();
	}
	
	for (std::vector<Server *>::iterator it = active_servers.begin(); it != active_servers.end(); it++)
	{
		delete *it;
	}
}
