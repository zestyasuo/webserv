#include <Server.hpp>
// #include "../inc/Config_proto.hpp"
#include <Logger.hpp>
# include <exception>

// t_conf g_conf;

int	main(int argc, char** argv, char **envp)
{
	// s_config(envp);
	Logger	server_logger;
	Server	*webserv = new Server(server_logger, "/home/zyasuo/21school/my_server/www/serv_a");
	std::vector<Server *>	active_servers;

	(void)argc;
	(void)argv;
	(void)envp;
	// g_conf.envp = envp;

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
