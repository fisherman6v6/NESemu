#include "debugger.hpp"


Debugger::Debugger(Cpu* cpu, Ppu* ppu, Mmu* mmu, Cartridge* cart, std::string logfile_path) :

        is_running_(false),
        cpu_(cpu), 
        ppu_(ppu), 
        mmu_(mmu),
        cartridge_(cart)

{
    if (!logfile_path.empty()) {
        log_on_file_ = true;
        DebugLogger::SetLogFile(logfile_path);
    }
    else {
        log_on_file_ = false;
    }

}

Debugger::~Debugger() {

}

void Debugger::Debug() {

    std::string line;
    is_running_ = true;

    while (is_running_) {

        Logger::Log("Next instruction at PC = 0x%04x", (unsigned)cpu_->GetRegisters()->getPC());
    
        std::cout << "Debug >> ";

        if (std::getline(std::cin, line)) {

            std::istringstream stream_(line);
            stream_.unsetf(std::ios::dec);
            std::string cmd;
            stream_ >> cmd;

            if (cmd == "step" || cmd == "s") {
                // execute one cpu step
                if (log_on_file_) {
                    DebugLogger::FileLogOp(cpu_->GetRegisters(), mmu_, cpu_->GetClockCycles());
                }
                cpu_->Step();
            }

            else if (cmd == "breakpoint" || cmd == "b") {
                // set breakpoint
                unsigned bline;
                if (stream_ >> bline) {
                    breakpoint_list_.push_back(std::make_pair(b_num_, bline));
                    b_num_++;
                    Logger::Log("Breakpoint set at 0x%04x", bline);
                }
            }

            else if (cmd == "print" || cmd == "p") {
                // print
                std::string to_print;
                if (stream_ >> to_print) {

                    if (to_print == "regs") {
                        // print cpu registers
                        DebugLogger::LogCpuRegisters(cpu_->GetRegisters());
                    }

                    else if (to_print == "mem") {
                        // print memory 
                        unsigned start, size;
                        if (stream_ >> start) {
                            if (stream_ >> size) {
                                DebugLogger::LogMemory(mmu_, start, size);
                            }
                            else {
                                DebugLogger::LogMemory(mmu_, start, 1);
                            }
                        }
                    }

                    else if (to_print == "b") {
                        // print breakpoint list
                        if (breakpoint_list_.empty()) {
                            Logger::Log("No brekpoint set");
                        }
                        else {
                            Logger::Log("Breakpoint list");
                            for (auto it = breakpoint_list_.begin(); it != breakpoint_list_.end(); it++) {
                                Logger::Log("b %u : 0x%04x",it->first, it->second);
                            }
                        }
                    }
                }
            }

            else if (cmd == "continue" || cmd == "c") {
                // continue execution until breakpoint is hit
                Continue();
            }

            else if (cmd == "help" || cmd == "h") {
                // print help
                DebugLogger::DebugHelp();
            }

            else if (cmd == "restart" || cmd == "r") {
                // run or restart execution
                cpu_->Reset();
            }

            else if (cmd == "d" || cmd == "delete") {
                // delete breakpoint
                unsigned to_delete;
	            if (stream_ >> to_delete) {
                    breakpoint_list_.remove_if([&to_delete](const std::pair<unsigned, unsigned>& element) { return element.first == to_delete; });
                }
            }

            else if (cmd == "quit" || cmd == "q") {
                Quit();
            }

            else {
                continue;
            }
        }
    }
}

void Debugger::SetBreakpoint() {

}

void Debugger::Print() {

}

void Debugger::DeleteBreakpoint() {

}

void Debugger::Continue() {
    unsigned pc = cpu_->GetRegisters()->getPC();
	do {
		cpu_->Step();
		pc = cpu_->GetRegisters()->getPC();
		
	} while (std::find_if(breakpoint_list_.begin(), breakpoint_list_.end(), 
			[&pc](const std::pair<unsigned, unsigned>& element) {return element.second == pc; }) == breakpoint_list_.end());
	Logger::Log("Breakpoint found at 0x%04x", (unsigned)cpu_->GetRegisters()->getPC());
}

void Debugger::Quit() {

    is_running_ = false;

}