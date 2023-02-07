#include "file_utils.hpp"

bool dirent_cmp(const dirent &a, const dirent &b)
{
	return strcmp(a.d_name, b.d_name) < 0;
}

vector<dirent> list_dir(const string &path, bool alsort = true)
{
	struct dirent *dir_ent;
	vector<dirent> result;

	DIR *dir = opendir(path.c_str());
	if (dir == NULL)
		throw std::exception();
	while ((dir_ent = readdir(dir)) != NULL)
		result.push_back(*dir_ent);
	closedir(dir);

	if (alsort)
		std::sort(result.begin(), result.end(), dirent_cmp);
	return result;
}

string	dir_list_formatted(const string &path, bool alsort)
{
	vector<dirent> dir_ent;
	string result;

	dir_ent = list_dir(path, alsort);

	for (vector <const dirent>::iterator it = dir_ent.begin(); it != dir_ent.end(); ++it)
	{
		//	d_type - not reliable field. Extra tests needed (Linux)
		if (it->d_type & DT_DIR)
			result += "&#128193; ";
		if (it->d_type & DT_REG)
			result += "&#x1F4C4; ";
		result += "<a href=\"";
		result += it->d_name;
		result += "\">";
		result += it->d_name;
		result += "</a>";
		result += "<br>\n";
	}
	return result;
}
