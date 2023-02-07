#include "../inc/html.hpp"

#define HTML_HEAD	"<!DOCTYPE html>\n"\
					"<!-- webserv pre-alpha 0.00001 -->\n"\
					"<HTML>\n"\
					"<style>"\
					"body {font-family: Helvetica;}\n"\
					"a {color: #0073E6; text-decoration:none; text-height:1.5ex}\n"\
					"a:hover {color: #00CCAA}\n"\
					"</style>"\
					"<BODY>\n"

#define HTML_FOOTER	"</BODY>\n"\
					"</HTML>\n"

void wrap_html_body(string &body)
{
	body.insert(0, HTML_HEAD);
	body.append(HTML_FOOTER);
}

//	ToDo: implement all entities
void decode_html_enities(string &str)
{
	while (str.find("%20") != str.npos)
		str.replace(str.find("%20"), 3, " ");

	std::cout << str;
}
