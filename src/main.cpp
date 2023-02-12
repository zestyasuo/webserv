#include "../inc/Config.hpp"
#include "ConfigStream.hpp"
#include "Router.hpp"
#include "utils.hpp"
#include <Logger.hpp>
#include <Server.hpp>
#include <exception>
#include <istream>
#include <limits.h>
#include <unistd.h>

// t_conf g_conf;

t_conf create_test_config(std::string const &server_name, int port)
{
	char webserv_root_dir[PATH_MAX];
	getcwd(webserv_root_dir, PATH_MAX);
	std::map< int, std::string > errors;

	// errors.insert(std::make_pair(200, ""));
	errors.insert(std::make_pair(501, "<html><h3>501 - not implemented!</h3></html>"));
	errors.insert(std::make_pair(404, "<html><h3>404 - not found!</h3></html>"));
	errors.insert(std::make_pair(405, "<html><h3>405 - method not allowed!</h3></html>"));
	errors.insert(std::make_pair(500, "<html><h3>500 - Internal Server Error!</h3></html>"));

	std::map< int, std::string > statuses;
	statuses.insert(std::make_pair(200, "OK"));
	statuses.insert(std::make_pair(501, "Not Implemented"));
	statuses.insert(std::make_pair(404, "Not Found"));
	statuses.insert(std::make_pair(405, "Method Not Allowed"));
	statuses.insert(std::make_pair(500, "Internal Server Error"));
	statuses.insert(std::make_pair(301, "Moved Permanently"));

	s_config   test_config;
	s_location test_location;
	s_location test_location2;

	test_config.error_pages = errors;
	test_config.status_texts = statuses;
	test_location2.methods = 0;
	test_location.methods = 0;

	test_config.name = "";
	test_location.root = "";
	test_location.path = "/";
	test_location.methods |= em_get | em_post | em_put;
	test_location.autoindex = true;
	test_location.rewrite = "";
	test_location.index_files.push_back("index.html");
	test_location.is_upload_allowed = true;

	test_location2.path = "/cat";
	test_location2.root = std::string(webserv_root_dir) + "/www/serv_b";
	// test_location2.root = "/home/zyasuo/21school/my_server/www/serv_b";
	test_location2.methods |= em_get | em_post | em_delete;
	test_location2.autoindex = true;
	test_location2.rewrite = "shit";
	test_location2.index_files.push_back("index1.html");
	test_location2.is_upload_allowed = true;

	s_location	loc_php;
	loc_php.path = "*.php";
	loc_php.root = "/usr/bin/php";
	test_config.locations.insert(std::make_pair(loc_php.path, loc_php));

	s_location	loc_py;
	loc_py.path = "*.py";
	// loc_py.root = "/usr/bin/python3";	//	Ubuntu
	loc_py.root = "/usr/bin/python3";
	test_config.locations.insert(std::make_pair(loc_py.path, loc_py));

	// test_config.root = "/home/zyasuo/21school/my_server/www/serv_a";
	test_config.root = std::string(webserv_root_dir) + "/www/serv_a";
	test_config.locations.insert(std::make_pair(test_location.path, test_location));
	test_config.locations.insert(std::make_pair(test_location2.path, test_location2));
	test_config.implemented_methods = em_get | em_post | em_delete | em_put;
	test_config.ports.push_back(port);
	if (!server_name.empty())
		test_config.name = server_name;

	process_cgi_loc(test_config);
	return test_config;
}

int main(int argc, char **argv, char **envp)
{
	if (argc != 2)
	{
		std::cout << "Usage: ./webserv config_file_path\n";
		return 1;
	}
	std::filebuf config_buf;
	if (!config_buf.open(argv[1],std::ios::in))
		throw std::exception();
	std::istream is(&config_buf);
	ConfigStream cs(is, envp);
	std::vector< t_conf > configs = cs.getConfigList();
	// std::vector< t_conf > configs;
	// configs.push_back(create_test_config("serv_a", 8090));
	// configs.push_back(create_test_config("serv_b", 8090));
	Router router(configs);

	(void)argc;
	(void)argv;
	(void)envp;
	while (true)
	{
		router.serve();
	}
}
