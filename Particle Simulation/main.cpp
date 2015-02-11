#include "Game.h"
#include <stdexcept>
#include <iostream>

int main()
{
	try
	{
		TheGame::Instance()->run();
	}
	catch (std::exception& e)
	{
		std::cout << std::endl << "EXCEPTION: " << e.what() << std::endl;
	}
}