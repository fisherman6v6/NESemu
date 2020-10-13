#pragma once
#include <string>
#include <list>
#include <sstream> 
#include <algorithm>
#include <utility>
#include "pch.hpp"
#include "cpu.hpp"
#include "mmu.hpp"
#include "ppu.hpp"
#include "cartridge.hpp"
#include "logger.hpp"
#include "debug_logger.hpp"

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

	/* System Components */
	std::shared_ptr<Cpu> cpu_;
	std::shared_ptr<Ppu> ppu_;
	std::shared_ptr<Mmu> mmu_;
	std::shared_ptr<Cartridge> cartridge_;

	bool is_running_;

	void Debug();
	void NoDebug();

	void(Emulator::*RunCallback)();
	
};

