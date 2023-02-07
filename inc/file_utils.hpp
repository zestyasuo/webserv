#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <dirent.h>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>

std::vector<dirent> list_dir(const std::string &path, bool alsort);
std::string	dir_list_formatted(const std::string &path, bool alsort);

#endif
