#include "file_utils.h"

vector<dirent> list_dir(const string &path)
{
	struct dirent *dir_ent;
	vector<dirent> result;

	DIR *dir = opendir(path.c_str());
	if (dir == NULL)
		throw std::exception();
	while ((dir_ent = readdir(dir)) != NULL)
		result.push_back(*dir_ent);
	closedir(dir);

	return result;
}
