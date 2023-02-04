#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <vector>
#include <string>
#include <dirent.h>

using std::vector;
using std::string;

vector<dirent> list_dir(const string &path);

#endif
