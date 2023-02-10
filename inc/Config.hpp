#ifndef Config_HPP
#define Config_HPP

#include <map>
#include <string>
#include <vector>

using std::map;
using std::string;

typedef struct s_config t_conf;

enum e_method
{
	em_get = 1,
	em_post = 1 << 1,
	em_delete = 1 << 2
};

struct s_location
{
	bool autoindex;						// = 1; autoindex (on / off == true / false) / mb check
										// ending slash
										// https://www.keycdn.com/support/nginx-directory-index
	std::string				   path;	// location [path] {}
	std::string				   root;
	int						   methods;		   // = 0 , enum / allowed_methods "GET POST DELETE"
	std::string				   rewrite;		   // rewrite
	std::vector< std::string > index_files;	   //	index ...,...
	// @key - value : extenstion - path
	std::map< std::string, std::string > cgi;
	bool								 is_upload_allowed;	   // upload [on / off]
	std::string							 upload_path;		   // upload_path ""
	map< string, string >				 extra_props;		   // reserved for unknown purposes
	size_t								 body_size_limit;	   // body_size_limit, bytes;
															   // error - path
};

struct s_config
{
	std::map< int, std::string >		error_pages;
	std::map< int, std::string >		status_texts;
	int									implemented_methods;
	map< std::string, std::string >		envp;		// parsed char **envp
	std::map< std::string, s_location > locations;	// location [std::string] {s_location}
	std::map< std::string, std::string > cgi;		// extension:cgi_path	{"php", "/usr/bin/php"}
	std::vector< int >					ports;		// listen
	std::string							name;		// server_name
	std::string							root;
};

std::string get_path_ext(const string &fname);
void process_cgi_loc(s_config &config);

#endif
