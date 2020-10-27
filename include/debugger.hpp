#pragma once
#include <list>
#include <string>
#include "cpu.hpp"
#include "mmu.hpp"
#include "ppu.hpp"
#include "cartridge.hpp"
#include "logger.hpp"

class Debugger {

public:

    static void Debug();
    
private:

    Debugger();
    ~Debugger();

    static bool is_running_;
    static std::list<std::pair<unsigned, unsigned>> breakpoint_list_;
	static unsigned b_num_;
	static std::string line_;
    static Cpu* cpu_;
    static Registers* registers_;
	static Ppu* ppu_;
	static Mmu* mmu_;
	static Cartridge* cartridge_;
};