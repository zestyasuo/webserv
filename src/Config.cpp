#include "Config.hpp"

s_location::s_location(const std::string &root, const std::string &path)
: autoindex(false)
, path(path)
, root(root)
, methods(em_get | em_post)
, is_upload_allowed(false)
, body_size_limit(-1)
{
    index_files.push_back("index.html");
}

s_location::~s_location() {}

s_config::s_config()
: implemented_methods(em_get | em_post | em_delete)
{}

s_config::~s_config() {}