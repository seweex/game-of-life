#include <string>
#include <iostream>

#include <SDL2/SDL.h>

#include "../include/game.h"

int main (int argc, char* argv[])
{
	using namespace GOFL;

	try {
		std::shared_ptr <Settings> settings;

		try {
			settings = std::make_shared<Settings>(argc, argv);
		}
		catch (std::invalid_argument const& exc) {
			std::cerr << exc.what() << std::endl;
			return -1;
		}

		Game game { settings };
		game.run();

		return 0;
	}
	catch (std::bad_alloc const&) {
		std::cerr << "Memory allocation failed" << std::endl;
		return -1;
	}
	catch (...) {
		std::cerr << "Unknown error" << std::endl;
		return -1;
	}
}