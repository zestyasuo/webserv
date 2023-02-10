#include "../inc/Config.hpp"
#include <iostream>

std::string get_path_ext(const string &fname)
{
	size_t pos;
	std::string ext;

	pos = fname.rfind(".");
	size_t	pos_end = fname.find("?", pos);

	if (pos != std::string::npos)
		ext = fname.substr(pos + 1, pos_end - pos - 1);
		// ext = fname.substr(pos + 1, fname.length() - pos - 1);
	// std::cout << "\t" << ext;
	return ext;
}

void split_query(std::string const &fname, std::string &full_path, std::string &query_str)
{
	size_t query_pos = fname.find('?');
	full_path = fname;
	if (query_pos != std::string::npos)
	{
		query_str = fname.substr(query_pos + 1, fname.length() - query_pos);
		full_path.erase(full_path.find('?'));
	}
}

//	store all extension based locations (*.php, *.py, ...)
//	to the single map {{"php","/usr/bin/php"}, {"py", "/usr/bin/python3"}}
void process_cgi_loc(s_config &config)
{
	std::string ext;

	for (std::map<std::string, s_location>::iterator loc = config.locations.begin();
		loc != config.locations.end(); ++loc)
	{
		if (loc->first.compare(0, 2, "*.") == 0)
		{
			ext = get_path_ext(loc->first);
			config.cgi[ext] = loc->second.root;
		}
	}
}
