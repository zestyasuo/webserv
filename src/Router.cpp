#include "../inc/Router.hpp"

Router::Router(std::vector< s_config > const &conf) : configs(conf)
{
	// open requested sockets
	Logger server_logger;
	for (std::vector< s_config >::iterator it = configs.begin(); it != configs.end(); it++)
	{
		for (std::vector< int >::iterator port_it = (*it).ports.begin(); port_it != (*it).ports.end(); port_it++)
		{
			if (!open_sockets.count(*port_it))
			{
				open_sockets.insert(std::make_pair< int, Socket * >(*port_it, new Socket(*port_it)));
				fds[open_sockets.size() - 1].fd = open_sockets[*port_it]->get_fd();
				fds[open_sockets.size() - 1].events = POLLIN | POLLOUT;
			}
		}
	}
	// create reqested servers
	for (std::vector< s_config >::iterator it = configs.begin(); it != configs.end(); it++)
	{
		for (std::vector< int >::iterator port_it = (*it).ports.begin(); port_it != (*it).ports.end(); port_it++)
		{
			if (!servers.count(open_sockets.at(*port_it)))
				servers.insert(std::make_pair< Socket *, std::vector< Server * > >(open_sockets.at(*port_it), std::vector< Server * >()));
			servers.at(open_sockets.at(*port_it)).push_back(new Server(server_logger, *it));
		}
		// for (std::map<int, Socket *>::iterator sock_it =
		// open_sockets.begin(); 	sock_it != open_sockets.end(); sock_it++)
		// {
		// 	for (std::vector<int>::const_iterator port_it =
		// servers.back()->get_config().ports.begin(); 		port_it !=
		// active_servers.back()->get_config().ports.end(); port_it++)
		// 	{
		// 		if (((*port_it) == (*sock_it).first))
		// 			active_servers.back()->add_socket(*sock_it);
		// 	}
		// }
	}
}

Router::~Router()
{
	for (std::map< Socket *, std::vector< Server * > >::iterator it = servers.begin(); it != servers.end(); it++)
	{
		for (std::vector< Server * >::iterator serv_it = (*it).second.begin(); serv_it != (*it).second.end(); serv_it++)
		{
			delete *serv_it;
		}
	}

	for (std::map< int, Socket * >::iterator it = open_sockets.begin(); it != open_sockets.end(); it++)
		delete (*it).second;

	for (std::vector< Query * >::iterator it = queries.begin(); it != queries.end(); it++)
		delete *it;
}

void Router::poll(void)
{
	try
	{

		if (::poll(fds, open_sockets.size(), TIMEOUT) < 0)
		{
			throw Webserv_exception("poll failied", ERROR);
		}

		for (size_t i = 0; i < open_sockets.size(); i++)
		{
			if (fds[i].revents & POLLIN)
			{
				Query *query = new Query(&fds[i]);
				queries.push_back(query);
			}
		}
	}
	catch (const Webserv_exception &e)
	{
		// logger.log(e.what(), e.get_error_code());
	}
}

void Router::collect(void)
{
	for (std::vector< Query * >::iterator it = queries.begin(); it != queries.end(); it++)
	{
		try
		{
			(*it)->recieve();
			if ((*it)->is_ready())
			{
				(*it)->form_request();
				//				std::cout << "formed\n\n";
				//				std::cout << (*(*it)->get_request());
			}
		}
		catch (const Webserv_exception &e)
		{
			delete *it;
			it = queries.erase(it);
			// logger.log(e.what(), e.get_error_code());
		}
	}
}

void Router::respond(void)
{
	// if (queries.empty())
	// 	return;
	for (std::vector< Query * >::iterator it = queries.begin(); it != queries.end();)
	{
		if (queries.empty())
			break;
		if ((*it)->get_request())
		{
			std::string host = "";
			if ((*it)->get_request()->get_headers().count("Host"))
				host = (*it)->get_request()->get_headers().at("Host");
			Socket *from_socket = NULL;
			for (std::map< int, Socket * >::iterator sock_it = open_sockets.begin(); sock_it != open_sockets.end(); sock_it++)
			{
				if ((*it)->get_socket() == (*sock_it).second->get_fd())
					from_socket = (*sock_it).second;
			}
			Server *respond_from = NULL;
			for (std::vector< Server * >::iterator host_it = servers.at(from_socket).begin(); host_it != servers.at(from_socket).end(); host_it++)
			{
				if (host.empty())
				{
					respond_from = *host_it;
					break;
				}
				if ((*host_it)->get_config().name == host)
				{
					respond_from = *host_it;
					break;
				}
			}
			if (!respond_from)
				respond_from = servers.at(from_socket).front();
			respond_from->respond((*it));
			delete *it;
			it = queries.erase(it);
		}
		else if ((*it)->is_ready())
		{
			delete *it;
			it = queries.erase(it);
		}
		else
			it++;
		// if ((*it)->is_ready())
		// {
		// 	delete (*it);
		// 	it = queries.erase(it);
		// }
	}
}

void Router::serve(void)
{
	poll();
	collect();
	respond();
}