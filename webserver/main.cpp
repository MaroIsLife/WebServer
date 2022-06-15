#include "socket.hpp"
// #include "Server.hpp"
#include "../configuration/config_file.hpp"
int main()
{
	try
	{
		ConfigFile _con("./webserv.conf");
		std::vector<Server > servers = _con.configuration;
		Mysocket obj;
		std::cout << "Njinx is running" << std::endl;
		signal(SIGPIPE, SIG_IGN);
		obj.start_server(servers);
		
		
	



		
		return 0;
		
	}
	catch(const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		std::cerr << e.what() << std::endl;
		std::cerr << e.what() << std::endl;
	}

	return (0);
}


//* Sockets
//* https://www.geeksforgeeks.org/socket-programming-cc/#:~:text=Socket%20programming%20is%20a%20way,reaches%20out%20to%20the%20server.

 