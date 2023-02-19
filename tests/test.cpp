//#include <stdio.h>
#include <iostream>
#include <cstring>    //	strcmp
#include <algorithm>
#include "../src/file_utils.h"

void list_dir_test()
{
	vector<dirent> dir_listing;

	dir_listing = list_dir(".", true);
	// std::sort(dir_listing.begin(), dir_listing.end(),
	// 	[](auto a, auto b)
	// 	{
	// 		return strcmp(a.d_name, b.d_name) < 0;
	// 	});

	for (auto const& dir_ent : dir_listing)
	{
		//	d_type - not reliable field. Extra tests needed (Linux)
		if (dir_ent.d_type & DT_DIR)
			std::clog << "[DIR] ";
		if (dir_ent.d_type & DT_REG)
			std::clog << "[   ] ";
		std::clog << dir_ent.d_name << std::endl;
	}
}

int main()
{
	std::clog << std::endl << "\tlist_dir_test" << std::endl << std::endl;
	list_dir_test();

	return 0;
}
