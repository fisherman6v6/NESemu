// NESemu.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "emulator.hpp"
#include <string>
#include <cstring>
#include <vector>

constexpr bool DEBUG = true;
constexpr bool NODEBUG = false;

void PrintCommands() {

}

bool ParseArgs(int argc, char **argv, bool& mode, std::string& path) {
	
	if (argc < 2) {
		PrintCommands();
		return true;
	}

	int cur = 0;

	std::vector<std::string> args;
	args.assign(argv + 1, argc + argv);

	while (cur < argc - 1) {

		if (args[cur] == "-help" || args[cur] == "-h") {
			PrintCommands();
			return true;
		}

		if (args[cur] == "-d" || args[cur] == "-debug") {
			mode = DEBUG;
			cur++;
		}

		// other args can be added 

		path = args[cur++];
	}

	return false;
}

int main(int argc, char **argv)
{
	bool mode;
	std::string path;

	std::cout << argv[0] << " " << argv[1] << " " << argv[2] << std::endl;

	if (!ParseArgs(argc, argv, mode, path)) {
		Emulator emulator(mode, path);
		emulator.Run();
	}

}
