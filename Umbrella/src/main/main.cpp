#include "hack.h"

#include <iostream>
#include <thread>

int main(int argc, char* argv[]) {
	Hack* pHack = new Hack();
	try {
		pHack->run();
		delete pHack;
	}
	catch (const std::exception& ex) {
		delete pHack;
		std::cerr << ex.what() << std::endl;

		return -1;
	}

	return 0;
}
