// NESemu.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "emulator.hpp"
#include <string>
#include <cstring>
#include <vector>

constexpr bool DEBUG = true;
constexpr bool NODEBUG = false;

#define DEFAULT_LOGFILE "my_log.txt"

void PrintCommands() {
	std::cout << "Commands:" << std::endl;
}

bool ParseArgs(int argc, char **argv, bool& mode, std::string& path, std::string& logfile_path) {
	
	if (argc < 2) {
		PrintCommands();
		return true;
	}

	int cur = 0;

	std::vector<std::string> args;
	args.assign(argv + 1, argc + argv);

	while (cur < argc - 1) {

		if (args[cur] == "-help" || args[cur] == "-h" || argc < 2) {
			PrintCommands();
			return true;
		}

		if (args[cur] == "-d" || args[cur] == "-debug") {
			mode = DEBUG;
			cur++;
			continue;
		}

		path = args[cur++];
		// other args can be added
		if (cur == argc - 1) {
			logfile_path = DEFAULT_LOGFILE;
			return false;
		} 

		logfile_path = args[cur++];
		return false;
		
	}

	return true;
}

int main(int argc, char **argv)
{
	bool mode;
	std::string path;
	std::string logfile_path;

	if (!ParseArgs(argc, argv, mode, path, logfile_path)) {
		std::cout << "rom path: " << path << "\n" << "logfile path: " << logfile_path << std::endl;
		Emulator emulator(mode, path, logfile_path);
		emulator.Run();
	}

}
