#pragma once
#include <iostream>
#include <list>
#include <string>
#include "cpu.hpp"
#include "mmu.hpp"
#include "ppu.hpp"
#include "cartridge.hpp"
#include "logger.hpp"

class Debugger {

public:

    static void Debug(Cpu* cpu, Ppu* ppu, Mmu* mmu, Cartridge* cart);
    
private:

    Debugger();
    ~Debugger();

    static void SetBreakpoint();
    static void Print();
    static void Continue();
    static void Help();
    static void Restart();
    static void DeleteBreakpoint();
    static void Quit();

    static bool is_running_;
    static std::list<std::pair<unsigned, unsigned>> breakpoint_list_;
	static unsigned b_num_;
	static std::string line_;
    static std::istringstream stream_;
    static Cpu* cpu_;
    static Registers* registers_;
	static Ppu* ppu_;
	static Mmu* mmu_;
	static Cartridge* cartridge_;
};