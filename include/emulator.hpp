#pragma once
#include "pch.hpp"
#include "cpu.hpp"
#include "logger.hpp"
#include <memory>
#include <string>
#include <list>
#include <vector>
#include <sstream> 
#include <algorithm> 

constexpr auto TIME_PER_FRAME = (1.0 / 60.0);
constexpr auto CYCLES_PER_FRAME = 29780;

class Emulator
{
public:
	Emulator();
	Emulator(bool mode, const std::string& path);
	~Emulator();
	void Step();
	void Run();
private:

	bool debug_mode_;
	const std::string path_;
	uint64_t clock_cycles_;
	std::unique_ptr<Cpu> cpu_;
	bool is_running_;

	void Debug();
	void NoDebug();

	void(Emulator::*RunCallback)();
	
};

