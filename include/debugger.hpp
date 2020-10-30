#pragma once
#include <iostream>
#include <list>
#include <string>
#include "cpu.hpp"
#include "mmu.hpp"
#include "ppu.hpp"
#include "cartridge.hpp"
#include "logger.hpp"
#include "args_queue.h"

class Debugger {

public:

    Debugger(Cpu* cpu, Ppu* ppu, Mmu* mmu, Cartridge* cart, const std::string &logfile_path = "");

    ~Debugger();

    void Debug();
    void Debug2();
    
private:
    int ArgsParser();
    void Step();
    void SetBreakpoint();
    void Print();
    void Continue();
    void Help();
    void Restart();
    void DeleteBreakpoint();
    void Quit();    
    bool is_running_;
    bool log_on_file_;
    std::list<std::pair<unsigned, unsigned>> breakpoint_list_;
	unsigned b_num_;
    ArgsQueue args_;

    Cpu* cpu_;
	Ppu* ppu_;
	Mmu* mmu_;
	Cartridge* cartridge_;
};