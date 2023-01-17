#include <Server.hpp>
#include <Logger.hpp>
# include <exception>




int	main(void) 
{
	Logger	server_logger;
	Server	webserv(server_logger);

	webserv.add_socket(8080);
	webserv.serve();

	// std::vector<int> vec;

	// vec.push_back(1);
	// vec.push_back(2);
	// vec.push_back(3);
	// vec.push_back(4);
	// vec.push_back(5);
	// vec.push_back(6);
	// for (std::vector<int>::iterator it = vec.begin(); it != vec.end(); it++)
	// {
	// 	try
	// 	{
	// 		std::cout << *it << "\n";
	// 		throw std::exception();
	// 	}
	// 	catch(const std::exception& e)
	// 	{
	// 		vec.erase(it);
	// 		it--;
	// 		// std::cerr << e.what() << '\n';
	// 	}
	// }
	// if (vec.empty())
	// 	std::cout << "empty\n";
}
