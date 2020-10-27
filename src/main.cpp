// NESemu.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <unistd.h>
#include "emulator.hpp"
#include <string>
#include <cstring>
#include <vector>

constexpr bool DEBUG = true;
constexpr bool NODEBUG = false;

#define DEFAULT_LOGFILE "my_log.txt"

void PrintCommands(char** argv) {
	std::cerr << "Usage: %s [-r|-d] [-l logfile] rompath " << argv[0] << std::endl;
}

bool ParseArgs(int argc, char **argv, bool& mode, std::string& path, std::string& logfile_path) {
	
	if (argc < 2) {
		PrintCommands(argv);
		return true;
	}

	int opt;
	while ((opt = getopt(argc, argv, "rdl:")) != -1) {
		switch (opt)
		{
		case 'r':
			/* Run emulator*/
			mode = NODEBUG;
			break;
		case 'd':
			/* Debug emualtor*/
			mode = DEBUG;
			break;
		case 'l':
			/* Optional logfile*/
			logfile_path = std::string(optarg);
			break;	
		default:
			PrintCommands(argv);
			return true;
		}
	}

	if (optind >= argc) {
        std::cerr << "Expected argument after options" << std::endl;
		PrintCommands(argv);
        return true;
	}

	path = std::string(argv[optind]);

	return false;
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

	else {
		exit(EXIT_FAILURE);
	}

	return 0;

}
