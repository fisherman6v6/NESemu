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

class DebugLogger {

public:

    static void LogCpuRegisters(const std::unique_ptr<Registers>& registers);
	static void LogMemory(const std::shared_ptr<Mmu>& memory, size_t start, size_t size, const char* tag = nullptr);
	static void LogOp(const std::unique_ptr<Registers>& registers, const std::shared_ptr<Mmu>& mmu, uint64_t cyc);
	static void Enable();
	static void Disable();
	static void DebugHelp();

private:
    ~DebugLogger();
	static bool is_enabled_;
	static std::ofstream log_file_;
};