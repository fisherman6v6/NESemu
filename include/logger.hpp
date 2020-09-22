#pragma once
#include <iostream>
#include <string>
#include <memory>
#include "cpu.hpp"
#include "registers.hpp"
#include "mmu.hpp"

class Logger
{
public:

	static void LogCpuRegisters(const std::unique_ptr<Registers>& registers);
	static void LogMemory(const std::unique_ptr<Mmu>& memory, uint16_t start, uint16_t size, const char* tag = nullptr);
	static void Enable() { is_enabled_ = 1; };
	static void Disable() { is_enabled_ = 0; };
	static void DebugHelp();

private:
	static bool is_enabled_; 
	Logger() {};
};

