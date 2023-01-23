#include <Server.hpp>
#include "../inc/Config_proto.hpp"
#include <Logger.hpp>
# include <exception>

// t_conf g_conf;

t_conf	create_test_config(void)
{
	s_config	test_config;
	s_location	test_location;

	test_location.root = "/home/zyasuo/21school/my_server/www/serv_a";
	test_location.methods += em_get | em_post;
	test_location.dir_listing = true;
	test_location.redirect = "";
	test_location.is_upload_allowed = true;

	test_config.locations.insert(std::make_pair("/", test_location));

	return test_config;
}

int	main(int argc, char** argv, char **envp)
{
	Logger	server_logger;
	Server	*webserv = new Server(server_logger, create_test_config());
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
