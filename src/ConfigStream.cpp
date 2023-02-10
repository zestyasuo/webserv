#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <limits>
#include <locale>
#include "ConfigStream.hpp"

ConfigStream::ConfigStream(std::istream &input, char **envp)
: std::istream(input) {
    this->exceptions(ConfigStream::failbit | ConfigStream::badbit);
    while (*envp)
    {
        std::stringstream ss;
        ss << *envp;

        std::string var;
        std::getline(ss, var, '=');
        if (var.empty() || !ss.good())
            throw std::invalid_argument(
                std::string("Invalid invironment variable: ")
                + std::string(*envp) );

        std::string val;
        std::getline(ss, val);

        g_cfg.envp[var] = val;
        envp++;
    }
}

std::string &ConfigStream::next_tok()
{
    token.clear();
    *this >> std::ws;
    while(this->good())
    {
        int sym = this->peek();
        switch (sym) {
            case EOF:
                return token;
            case '#':
                this->ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                if (!token.empty())
                    return token;
                break;
            case ';':
            case ',':
            case '{':
            case '}':
                if (token.empty())
                    token.push_back(this->get());
                return token;
            default:
                if (std::isspace(sym))
                {
                    if (token.empty())
                        *this >> std::ws;
                    else
                        return token;
                }
                else
                    token.push_back(this->get());
        }
    }
    return token;
}

std::string &ConfigStream::cur_tok()
{
    return token;
}

static t_conf parse_server(ConfigStream &cs)
{
    if (cs.next_tok())
}


std::vector<t_conf> ConfigStream::getConfigList()
{
    std::vector<t_conf> cfgl;

    for (this->next_tok(); !this->eof(); this->next_tok())
    {
        if (token != "server")
            throw std::invalid_argument("Invalid token: " + token);
        t_conf cfg;
    }
}

#define CONF_TEST 1
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
