#ifndef CONFIG_PROTO_HPP
#define CONFIG_PROTO_HPP

#include <string>
#include <map>
# include <vector>

using std::string;
using std::map;

typedef struct s_config t_conf;

enum e_method
{
	em_get = 1,
	em_post = 1 << 1,
	em_delete = 1 << 2
};

struct s_location
{
	bool								dir_listing; // = 1; autoindex (on / off == true / false) / mb check ending slash https://www.keycdn.com/support/nginx-directory-index
	std::string							root; // location [root]
	int									methods; // = 0 , enum / allowed_methods "GET POST DELETE"
	std::string							redirect; // redirect
	std::vector<std::string>			index_files;	//	index [...,]
	// @key - value : extenstion - path
	std::map<std::string, std::string>	cgi;
	bool								is_upload_allowed; // upload [on / off]
	std::string							upload_path; // upload_path ""
	map<string, string>					extra_props; // reserved for unknown purposes
	size_t								body_size_limit; // body_size_limit, bytes;
	// error - path
	std::map<int, std::string>			error_pages;
};

struct s_config
{
	map<std::string, std::string>			envp; // parsed char **envp
	std::map<std::string, s_location>		locations;
	std::vector<int>						ports; // listen
	std::string								name; // server_name
};

#endif
