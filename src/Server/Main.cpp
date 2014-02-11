#include <iostream>
#include "Server\Server.h"

int main()
{
	try
	{
		Server server;
		server.run();
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << "\n";
	}

	std::cin.get();	
}