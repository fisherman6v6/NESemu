// NESemu.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "emulator.hpp"
#include <string>
#include <cstring>
#include <vector>

constexpr bool DEBUG = 1;
constexpr bool NODEBUG = 0;

void PrintCommands() {

}

bool ParseArgs(int argc, char **argv, bool& mode, std::string& path) {
	
	if (argc < 2) {
		PrintCommands();
		return 1;
	}

	int cur = 0;

	std::vector<std::string> args;
	args.assign(argv + 1, argc + argv);

	while (cur < argc - 1) {

		if (args[cur] == "-help" || args[cur] == "-h") {
			PrintCommands();
			return 1;
		}

		if (args[cur] == "-d" || args[cur] == "-debug") {
			mode = DEBUG;
			cur++;
		}

		// other args can be added 

		path = args[cur++];


	}

	return 0;
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

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
