#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include "../inc/Server.hpp"
#include <vector>
#include <cstring>
#include <dirent.h>

using std::vector;
using std::string;

vector<dirent> list_dir(const string &path, bool alsort);
string	dir_list_formatted(const string &path, const HTTPRequest *req, bool alsort);

#endif
