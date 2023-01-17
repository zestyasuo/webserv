#include <string>
#include <map>

using std::string;
using std::map;

struct s_location
{
	string loc;
	string root;
	map<string, string> extra_props;
};