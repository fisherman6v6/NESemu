#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <fstream>
#include <iomanip>
#include "cpu.hpp"
#include "mmu.hpp"
#include "registers.hpp"

class Mmu;

class DebugLogger {

public:

    static void LogCpuRegisters(const Registers* registers);
	static void LogMemory(const Mmu* memory, size_t start, size_t size, const char* tag = nullptr);
	static void FileLogOp(const Registers* registers, const Mmu* mmu, uint64_t cyc);
	static void Enable() { is_enabled_ = true; };
    static void Disable() { is_enabled_ = false; };
	static void DebugHelp();
	static void SetLogFile(const std::string& logfile_path) { logfile_path_ = logfile_path; };

private:
    ~DebugLogger();
	static bool is_enabled_;
	static std::ofstream logfile_;
	static std::string logfile_path_;
};