#ifndef CONFIG_STREAM_HPP
#define CONFIG_STREAM_HPP

#include <map>
#include <string>
#include <vector>
#include <istream>
#include "Config.hpp"

class ConfigStream;
static t_conf parse_server(ConfigStream &cs);


class ConfigStream : protected std::istream {
	std::string token;

	// ConfigStream(const ConfigStream &other);
	// ConfigStream &operator=(const ConfigStream &other);
	std::string &next_tok();
	std::string &cur_tok();
	t_conf get_config();
	friend t_conf parse_server(ConfigStream &cs);
	public:
		ConfigStream(std::istream &input, char **envp);
		~ConfigStream();
		std::vector<t_conf> getConfigList();
};

#endif
