#include <Server.hpp>
#include <Logger.hpp>




int	main(void) 
{
// 	HTTPRequest reqest("POST /cgi-bin/process.cgi HTTP/1.1	\n \
// User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)	\n\
// Host: www.tutorialspoint.com	\n\
// Content-Type: application/x-www-form-urlencoded	\n\
// Content-Length: length	\n\
// Accept-Language: en-us	\n\
// Accept-Encoding: gzip, deflate	\n\
// Connection: Keep-Alive	\n\
// \n\
// licenseID=string&content=string&/paramsXML=string\n");

// 	std::cout << reqest;
	Logger	server_logger;
	Server	webserv(server_logger);

	webserv.add_socket(8080);
	webserv.serve();

}