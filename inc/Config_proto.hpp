#ifndef CONFIG_PROTO_HPP
#define CONFIG_PROTO_HPP

#include <string>
#include <map>

using std::string;
using std::map;

typedef struct s_config t_conf;

struct s_config
{
	char **envp;
};

struct s_location
{
	string loc;
	string root;
	map<string, string> extra_props;
};

#endif
