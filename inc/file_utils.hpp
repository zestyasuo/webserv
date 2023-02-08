#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include "../inc/Server.hpp"
#include <cstring>
#include <dirent.h>
#include <vector>

using std::string;
using std::vector;

vector< dirent > list_dir(const string &path, bool alsort);
string			 dir_list_formatted(const string &path, const HTTPRequest *req, bool alsort);

#endif
