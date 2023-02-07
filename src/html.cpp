#include "../inc/html.hpp"

#define HTML_HEAD	"<!DOCTYPE html>\n"\
					"<!-- webserv pre-alpha 0.00001 -->\n"\
					"<HTML>\n"\
					"<BODY>\n"

#define HTML_FOOTER	"</BODY>\n"\
					"</HTML>\n"

void wrap_html_body(string &body)
{
	body.insert(0, HTML_HEAD);
	body.append(HTML_FOOTER);
}
