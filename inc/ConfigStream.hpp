#ifndef CONFIG_STREAM_HPP
#define CONFIG_STREAM_HPP

#include <map>
#include <string>
#include <vector>
#include <istream>
#include "Config.hpp"

class ConfigStream {
	std::istream &is;
	std::string token;
	map< std::string, std::string > env;

	// ConfigStream(const ConfigStream &other);
	// ConfigStream &operator=(const ConfigStream &other);
	t_conf parse_server();
	public:
		const std::string &next_tok();
		const std::string &cur_tok();
		ConfigStream(std::istream &input, char **envp);
		~ConfigStream();
		std::vector<t_conf> getConfigList();
		operator bool();
};

#endif
