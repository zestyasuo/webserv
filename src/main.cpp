#include <Server.hpp>
#include "../inc/Config_proto.hpp"
#include <Logger.hpp>
# include <exception>

// t_conf g_conf;

t_conf	create_test_config(std::string const &server_name, int port)
{
	s_config	test_config;
	s_location	test_location;
	s_location	test_location2;

	test_location2.methods = 0;
	test_location.methods = 0;

	test_config.name = "";
	test_location.path = "/";
	test_location.methods |= em_get | em_post;
	test_location.autoindex = true;
	test_location.rewrite = "";
	test_location.index_files.push_back("index.html");
	test_location.is_upload_allowed = true;

	test_location2.path = "/cat";
	test_location2.methods |= em_get | em_post | em_delete;
	test_location2.autoindex = true;
	test_location2.rewrite = "";
	test_location2.index_files.push_back("index.html");
	test_location2.is_upload_allowed = true;

	test_config.root = "/home/zyasuo/21school/my_server/www/serv_a";
	test_config.locations.insert(std::make_pair(test_location.path, test_location));
	test_config.locations.insert(std::make_pair(test_location2.path, test_location2));
	test_config.implemented_methods = em_get | em_post | em_delete;
	test_config.ports.push_back(port);
	if (!server_name.empty())
		test_config.name = server_name;
	return test_config;
}

int	main(int argc, char** argv, char **envp)
{
	// if (argc != 2)
	// {
	// 	std::cout << "Usage: ./webserv config_file_path\n";
	// 	return 1;
	// }
	Logger	server_logger;
	std::vector<Server *>	active_servers;
	std::vector<s_config> configs; // parsed server configs
	std::map<int, Socket *>	open_sockets;

	configs.push_back(create_test_config("serv_a", 8080));
	configs.push_back(create_test_config("serv_b", 8090));
	// Server	*webserv = new Server(server_logger, create_test_config());

	(void)argc;
	(void)argv;
	(void)envp;

	for(std::vector<s_config>::iterator it = configs.begin(); it != configs.end(); it++)
	{
		for (std::vector<int>::iterator port_it = (*it).ports.begin(); port_it != (*it).ports.end(); port_it++)
		{
			if (!open_sockets.count(*port_it))
				open_sockets.insert(std::make_pair<int, Socket *>(*port_it, new Socket(*port_it)));
		}
	}

	for(std::vector<s_config>::iterator it = configs.begin(); it != configs.end(); it++)
	{
		active_servers.push_back(new Server(server_logger, (*it)));
		for (std::map<int, Socket *>::iterator sock_it = open_sockets.begin();
			sock_it != open_sockets.end(); sock_it++)
		{
			for (std::vector<int>::const_iterator port_it = active_servers.back()->get_config().ports.begin();
				port_it != active_servers.back()->get_config().ports.end(); port_it++)
			{
				if (((*port_it) == (*sock_it).first))
					active_servers.back()->add_socket(*sock_it);
			}
		}
	}

	while (true){
		for (std::vector<Server *>::iterator it = active_servers.begin(); it != active_servers.end(); it++)
			(*it)->serve();
	}

	for (std::vector<Server *>::iterator it = active_servers.begin(); it != active_servers.end(); it++)
		delete *it;

	for (std::map<int, Socket *>::iterator it = open_sockets.begin(); it != open_sockets.end(); it++)
		delete (*it).second;
}
