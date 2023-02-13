#include "../inc/file_utils.hpp"
#include "Webserv_exception.hpp"
#include <cerrno>

bool dirent_cmp(const dirent &a, const dirent &b)
{
	return strcmp(a.d_name, b.d_name) < 0;
}

std::vector< dirent > list_dir(const std::string &path, bool alsort = true)
{
	struct dirent		 *dir_ent = NULL;
	std::vector< dirent > result;

	DIR *dir = opendir(path.c_str());
	if (dir == NULL)
		throw Webserv_exception("Dir cannot be opened", ERROR);
	while ((dir_ent = readdir(dir)) != NULL)
		result.push_back(*dir_ent);
	closedir(dir);

	if (alsort)
		std::sort(result.begin(), result.end(), dirent_cmp);
	return result;
}

string dir_list_formatted(const string &path, const HTTPRequest *req, bool alsort)
{
	std::vector< dirent > dir_ent;
	std::string			  result;

	dir_ent = list_dir(path, alsort);

	for (std::vector< dirent >::iterator it = dir_ent.begin(); it != dir_ent.end(); ++it)
	{
		// full_path = path + "/" + it->d_name;

		//	d_type - not reliable field. Extra tests needed (Linux)
		result += "<a href=\"";
		result += req->get_target() + "/" + it->d_name;
		decode_html_enities(result);
		result += "\">";
		if (it->d_type & DT_DIR)
			result += "&#x1F4C1; ";
		if (it->d_type & DT_REG)
			result += "&#x1F4C4; ";
		result += it->d_name;
		result += "</a>";
		result += "<br>\n";
	}
	return result;
}
