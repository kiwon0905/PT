#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG   
#ifndef DBG_NEW      
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )      
#define new DBG_NEW   
#endif
#endif  // _DEBUG

#define TrojanMain main

#include <iostream>
#include "Client/Application.h"
#include "Client/SplashState.h"


int TrojanMain()
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	try
	{
		Application app;
		app.push(new SplashState);
		app.run();
	}
	catch (std::exception & e)
	{
		std::cout << e.what()<<"\n";
	}
	std::cin.get();
}
#undef TrojanMain



