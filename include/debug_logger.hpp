#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <fstream>
#include <iomanip>
#include "cpu.hpp"
#include "mmu.hpp"
#include "registers.hpp"

#define LOGFILE "log/my_log.txt"

class Mmu;

class DebugLogger {

public:

    static void LogCpuRegisters(const Registers* registers);
	static void LogMemory(const Mmu* memory, size_t start, size_t size, const char* tag = nullptr);
	static void FileLogOp(const Registers* registers, const Mmu* mmu, uint64_t cyc);
	static void Enable();
	static void Disable();
	static void DebugHelp();

private:
    ~DebugLogger();
	static bool is_enabled_;
	static std::ofstream log_file_;
};