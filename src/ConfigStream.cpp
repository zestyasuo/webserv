#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <limits>
#include <locale>
#include "ConfigStream.hpp"

static bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

ConfigStream::ConfigStream(std::istream &input, char **envp)
: is(input) {
    is.exceptions(std::istream::failbit | std::istream::badbit);
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

        env[var] = val;
        envp++;
    }
}

ConfigStream::~ConfigStream() {};

ConfigStream::operator bool()
{
    return is.good();
}

const std::string &ConfigStream::next_tok()
{
    token.clear();
    is >> std::ws;
    while(is.good())
    {
        int sym = is.peek();
        switch (sym) {
            case EOF:
                return token;
            case '#':
                is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                if (!token.empty())
                    return token;
                break;
            case ';':
            case ',':
            case '{':
            case '}':
                if (token.empty())
                    token.push_back(is.get());
                return token;
            default:
                if (std::isspace(sym))
                {
                    if (token.empty())
                        is >> std::ws;
                    else
                        return token;
                }
                else
                    token.push_back(is.get());
        }
    }
    return token;
}

const std::string &ConfigStream::cur_tok()
{
    return token;
}

bool get_bool_directive(ConfigStream &cs)
{   
    std::string val = cs.next_tok();
    bool result;
    if (val == "on" || val == "true" || val == "1")
        result = true;
    else if (val == "off" || val == "false" || val == "0")
        result = false;
    else
        throw std::invalid_argument("Unexpected token: " + cs.cur_tok());
    if (cs.next_tok() != ";")
        throw std::invalid_argument("Unexpected token: " + cs.cur_tok());
    return result;
}

bool get_autoindex(ConfigStream &cs)
{
    if (cs.cur_tok() != "autoindex")
        throw std::logic_error("IMPOSSIBLE: expected: autoindex got: " + cs.cur_tok());
    return get_bool_directive(cs);
}

bool get_upload(ConfigStream &cs)
{
    if (cs.cur_tok() != "upload")
        throw std::logic_error("IMPOSSIBLE: expected: upload got: " + cs.cur_tok());
    return get_bool_directive(cs);
}

std::string get_string_derictive(ConfigStream &cs)
{
    std::string root = cs.next_tok();

    if (cs.next_tok() != ";")
        throw std::invalid_argument("Unexpected token: " + cs.cur_tok());
    
    return root;
}

std::string get_root(ConfigStream &cs)
{
    if (cs.cur_tok() != "root")
        throw std::logic_error("IMPOSSIBLE: expected: root got: " + cs.cur_tok());

    return get_string_derictive(cs);
}

std::string get_rewrite(ConfigStream &cs)
{
    if (cs.cur_tok() != "rewrite")
        throw std::logic_error("IMPOSSIBLE: expected: rewrite got: " + cs.cur_tok());

    return get_string_derictive(cs);
}

std::string get_upload_path(ConfigStream &cs)
{
    if (cs.cur_tok() != "upload_path")
        throw std::logic_error("IMPOSSIBLE: expected: upload_path got: " + cs.cur_tok());

    return get_string_derictive(cs);
}

std::string get_server_name(ConfigStream &cs)
{
    if (cs.cur_tok() != "server_name")
        throw std::logic_error("IMPOSSIBLE: expected: server_name got: " + cs.cur_tok());

    return get_string_derictive(cs);
}

ssize_t get_body_size_limit(ConfigStream &cs)
{
    if (cs.cur_tok() != "body_size_limit")
        throw std::logic_error("IMPOSSIBLE: expected: body_size_limit got: " + cs.cur_tok());
    
    if (!is_number(cs.next_tok()))
            throw std::invalid_argument("Expected numeric token, got: " + cs.cur_tok());
    ssize_t val;
    std::istringstream(cs.cur_tok()) >> val;
    if (cs.next_tok() != ";")
        throw std::invalid_argument("Unexpected token: " + cs.cur_tok());
    
    return val;
}

void get_listen(ConfigStream &cs, std::vector<int> &ports)
{
    if (cs.cur_tok() != "listen")
        throw std::logic_error("IMPOSSIBLE: expected: listen got: " + cs.cur_tok());
    
    bool has_parsed = false;
    for (cs.next_tok(); cs && cs.cur_tok() != ";"; cs.next_tok())
    {
        if (has_parsed && cs.cur_tok() == ",")
            cs.next_tok();
        
        if (!is_number(cs.cur_tok()))
            throw std::invalid_argument("Expected numeric token, got: " + cs.cur_tok());
        int val;
        std::istringstream(cs.cur_tok()) >> val;
        if (val < 1 || val > 65535)
            throw std::out_of_range("Port number must be between 1 and 65535, got: " + cs.cur_tok());
        ports.push_back(val);
        has_parsed = true;
    }
    if (cs.cur_tok() != ";")
        throw std::invalid_argument("Unexpected token: " + cs.cur_tok());
    if (!has_parsed)
        throw std::invalid_argument("Listen direvtive must have at least one port!");
}

int get_methods(ConfigStream &cs)
{
    if (cs.cur_tok() != "methods")
            throw std::invalid_argument("Unexpected token: " + cs.cur_tok());

    int methods = 0;

    for (cs.next_tok(); cs && cs.cur_tok() != ";"; cs.next_tok())
    {
        if (methods && cs.cur_tok() == ",")
            cs.next_tok();
        
        if (cs.cur_tok() == "GET" && (methods & em_get) == 0)
            methods |= em_get;
        else if (cs.cur_tok() == "POST" && (methods & em_post) == 0)
            methods |= em_post;
        else if (cs.cur_tok() == "DELETE" && (methods & em_delete) == 0)
            methods |= em_delete;
        else
            throw std::invalid_argument("Unexpected token: " + cs.cur_tok());
    }
    if (cs.cur_tok() != ";")
        throw std::invalid_argument("Unexpected token: " + cs.cur_tok());
    return methods;
}

s_location get_location(ConfigStream &cs, const std::string &s_root)
{
    if (cs.cur_tok() != "location")
            throw std::invalid_argument("Unexpected token: " + cs.cur_tok());
    s_location loc(s_root, cs.next_tok());
    if (cs.next_tok() != "{")
        throw std::invalid_argument("Unexpected token: " + cs.cur_tok());
    for (cs.next_tok(); cs && cs.cur_tok() != "}"; cs.next_tok())
    {
        if (cs.cur_tok() == "autoindex")
            loc.autoindex = get_autoindex(cs);
        else if (cs.cur_tok() == "root")
            loc.root = get_root(cs);
        else if (cs.cur_tok() == "methods")
            loc.methods = get_methods(cs);
        else if (cs.cur_tok() == "rewrite")
            loc.rewrite = get_rewrite(cs);
        else if (cs.cur_tok() == "upload")
            loc.is_upload_allowed = get_upload(cs);
        else if (cs.cur_tok() == "upload_path")
            loc.upload_path = get_upload_path(cs);
        else if (cs.cur_tok() == "body_size_limit")
            loc.body_size_limit = get_body_size_limit(cs);
    }
    if (cs.cur_tok() != "}")
        throw std::invalid_argument("Unexpected token: " + cs.cur_tok());

    return loc;
}

t_conf ConfigStream::parse_server()
{
    if (token != "server" || this->next_tok() != "{")
            throw std::invalid_argument("Unexpected token: " + token);
    t_conf cfg;
    cfg.envp = env;
    cfg.implemented_methods = em_get | em_post | em_delete;
    for (this->next_tok(); !is.eof() && token != "}"; this->next_tok())
    {
        if (token == "root")
            cfg.root = get_root(*this);
        else if (token == "listen")
            get_listen(*this, cfg.ports);
        else if (token == "server_name")
            cfg.name = get_server_name(*this);
        else if (token == "location")
        {
            s_location tmp = get_location(*this, cfg.root);
            cfg.locations[tmp.path] = tmp;
        }
        
    }
    if (token != "}")
        throw std::invalid_argument("Unexpected token: " + token);
    if (cfg.ports.empty())
        throw std::invalid_argument("Server port must be specified!");
    if (cfg.root.empty())
        throw std::invalid_argument("Server root must be specified!");
    if (!cfg.locations.count("/"))
        cfg.locations["/"]=s_location(cfg.root);
        

    return cfg;
}


std::vector<t_conf> ConfigStream::getConfigList()
{
    if (token.empty() )
        this->next_tok();

    std::vector<t_conf> cfgl;
    do {
        cfgl.push_back(this->parse_server());
        this->next_tok();
    } while (!is.eof());

    return cfgl;
}

// #define CONF_TEST 1
#ifdef CONF_TEST

int main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;
    // std::stringstream ss;
    // std::string ss;
    std::ifstream ss("test_conf");
    ConfigStream cs(ss, envp);
    
    // ss << "    location / {\n"
    // "	autoindex on;,,\n"
  	// "}\n";

    // std::cout << ss << std::endl;
    
    do {
        std::cout << cs.next_tok() << std::endl;
    } while(ss.good() && !ss.eof());

    return 0;
}

#endif
