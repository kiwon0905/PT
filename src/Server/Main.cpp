#include <iostream>
#include "Server/Server.h"

#ifdef _DEBUG   
#ifndef DBG_NEW      
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )      
#define new DBG_NEW   
#endif
#endif  // _DEBUG

#include "Server/GameWorld.h"
#include "Shared/ValueParser.h"
int main()
{

#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
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