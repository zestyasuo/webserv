#include <Server.hpp>
#include "../inc/Config_proto.hpp"
#include <Logger.hpp>
# include <exception>

// t_conf g_conf;

t_conf	create_test_config(void)
{
	s_config	test_config;
	s_location	test_location;
	s_location	test_location2;

	test_location.path = "/";
	test_location.methods += em_get | em_post;
	test_location.autoindex = true;
	test_location.rewrite = "";
	test_location.index_files.push_back("index.html");
	test_location.is_upload_allowed = true;

	test_location2.path = "/cat";
	test_location2.methods += em_get | em_post | em_delete;
	test_location2.autoindex = true;
	test_location2.rewrite = "";
	test_location2.index_files.push_back("index.html");
	test_location2.is_upload_allowed = true;

	test_config.root = "/home/zyasuo/21school/my_server/www/serv_a";
	test_config.locations.insert(std::make_pair(test_location.path, test_location));
	test_config.locations.insert(std::make_pair(test_location2.path, test_location2));
	test_config.implemented_methods = em_get | em_post | em_delete;

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
