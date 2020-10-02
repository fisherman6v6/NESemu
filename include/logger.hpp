#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <fstream>
#include <iomanip>
#include <stdarg.h>
#include "cpu.hpp"
#include "registers.hpp"
#include "mmu.hpp"

#define LOGFILE "log/my_log.txt"

class Logger
{
public:

	static void Log(const char* message, ...);
	static void LogError(const char* message, ...);
	static void LogCpuRegisters(const std::unique_ptr<Registers>& registers);
	static void LogMemory(const std::unique_ptr<Mmu>& memory, size_t start, size_t size, const char* tag = nullptr);
	static void LogOp(const std::unique_ptr<Registers>& registers, const std::unique_ptr<Mmu>& mmu, uint64_t cyc);
	static void Enable();
	static void Disable();
	static void DebugHelp();

private:
	static bool is_enabled_; 
	static std::ofstream log_file;
	Logger() {};
};

