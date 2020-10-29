#include "debugger.hpp"

bool Debugger::is_running_ = false;

Debugger::Debugger() {

}

Debugger::~Debugger() {

}

void Debugger::Debug(Cpu* cpu, Ppu* ppu, Mmu* mmu, Cartridge* cartridge) {

    cpu_ = cpu;
    ppu_ = ppu;
    mmu_ = mmu;
    cartridge_ = cartridge;


    while (is_running_) {

        Logger::Log("Next instruction at PC = 0x%04x", (unsigned)cpu_->GetRegisters()->getPC());
    
        std::cout << "Debug >> ";

        if (std::getline(std::cin, line_)) {

            std::istringstream stream_(line_);
            stream_.unsetf(std::ios::dec);
            std::string cmd;
            stream_ >> cmd;

            if (cmd == "step" || cmd == "s") {
                // execute one cpu step
                cpu_->Step();
            }

            else if (cmd == "breakpoint" || cmd == "b") {
                // set breakpoint
                Debugger::SetBreakpoint();
            }

            else if (cmd == "print" || cmd == "p") {
                // print
            }

            else if (cmd == "continue" || cmd == "c") {
                // continue execution until breakpoint is hit
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
            }

            else if (cmd == "quit" || cmd == "q") {

            }

            else {
                continue;
            }
        }
    }
}

void Debugger::SetBreakpoint() {
    unsigned bline;
    if (stream_ >> bline) {
        breakpoint_list_.push_back(std::make_pair(b_num_, bline));
        b_num_++;
        Logger::Log("Breakpoint set at 0x%04x", bline);
    }
}

void Debugger::Print() {
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
                for (auto it = breakpoint_list_.begin(); it != breakpoint_list_.end(); it++) {
                    Logger::Log("b %u : 0x%04x",it->first, it->second);
                }
            }
		}
    }
}

void Debugger::DeleteBreakpoint() {

    unsigned to_delete;
	stream_ >> to_delete;
	breakpoint_list_.remove_if([&to_delete](const std::pair<unsigned, unsigned>& element) { return element.first == to_delete; });
    
}