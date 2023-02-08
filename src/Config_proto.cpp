#include <iostream>
#include <sstream>
#include <fstream>
#include <limits>
#include <locale>
#include "Config_proto.hpp"

std::string &get_token(std::istream &is, std::string &str)
{
    str.clear();
    is >> std::ws;
    while(is.good())
    {
        int sym = is.peek();
        switch (sym) {
            case EOF:
                return str;
            case '#':
                is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                if (!str.empty())
                    return str;
                break;
            case ';':
            case ',':
            case '{':
            case '}':
                if (str.empty())
                    str.push_back(is.get());
                return str;
            default:
                if (std::isspace(sym))
                {
                    if (str.empty())
                        is >> std::ws;
                    else
                        return str;
                }
                else
                    str.push_back(is.get());
        }
    }
    return str;
}

std::istream &operator>>(std::istream &is, s_location &loc)
{
    (void)loc;
    return is;
}

// #define CONF_TEST 1
#ifdef CONF_TEST

int main()
{
    // std::stringstream ss;
    // std::string ss;
    std::ifstream ss("test_conf");
    
    // ss << "    location / {\n"
    // "	autoindex on;,,\n"
  	// "}\n";

    // std::cout << ss << std::endl;
    std::string s;
    do {
        get_token(ss, s);
        std::cout << s << std::endl;
    } while(ss.good() && !ss.eof());

    return 0;
}

#endif