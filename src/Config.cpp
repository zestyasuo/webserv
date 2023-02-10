#include "../inc/Config.hpp"

std::string get_path_ext(const string &fname)
{
	size_t pos;
	std::string ext;

	pos = fname.rfind(".");
	if (pos != std::string::npos)
		ext = fname.substr(pos + 1, fname.length() - pos - 1);
	return ext;
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
