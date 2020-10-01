#include "emulator.hpp"

Emulator::Emulator()
{
	cpu_ = std::make_unique<Cpu>();
}

Emulator::Emulator(bool mode, const std::string& path)
{
	cpu_ = std::make_unique<Cpu>();
	mode ? RunCallback = &Emulator::Debug : RunCallback = &Emulator::NoDebug;
	is_running_ = false;
	cpu_->Init(path);
}

Emulator::~Emulator()
{
}

void Emulator::Step()
{
	cpu_->Step();
}

void Emulator::Run()
{
	is_running_ = true;

	if (RunCallback != nullptr) {
		(this->*RunCallback)();
	}
}

void Emulator::Debug()
{
	Logger::Enable();

	std::list<unsigned> breakpoint_list;
	std::string line;

	while (is_running_) {

		printf("Next instruction at PC = 0x%04x\n", (unsigned)cpu_->registers_->getPC());

		std::cout << "Debug >> ";

		if (std::getline(std::cin, line)) {

			std::istringstream stream(line);
			stream.unsetf(std::ios::dec);
			std::string cmd;
			stream >> cmd;

			if (cmd == "step" || cmd == "s") {
				// execute one cpu step
				Step();
			}
			else if (cmd == "breakpoint" || cmd == "b") {
				// set breakpoint
				unsigned bline;
				if (stream >> bline) {
					breakpoint_list.push_back(bline);
					printf("Breakpoint set at 0x%04x\n", bline);
				}
			}
			else if (cmd == "print" || cmd == "p") {
				// print
				std::string to_print;
				if (stream >> to_print) {
					if (to_print == "regs") {
						// print cpu registers
						Logger::LogCpuRegisters(cpu_->registers_);
					}
					else if (to_print == "mem") {
						// print memory 
						unsigned start, size;
						if (stream >> start) {
							if (stream >> size) {
								Logger::LogMemory(cpu_->mmu_, start, size);
							}
							else {
								Logger::LogMemory(cpu_->mmu_, start, 1);
							}
						}
					}
					else if (to_print == "b") {
						// print breakpoint list
						size_t cnt = 0;
						if (breakpoint_list.empty()) {
							std::cout << "No brekpoint set"<< std::endl;
						}
						else {
							for (auto it = breakpoint_list.begin(); it != breakpoint_list.end(); it++, cnt++) {
								std::cout << "b" << cnt <<" : " << std::hex << *it << "\n";
							}
						}
					}
				}
			}
			else if (cmd == "continue" || cmd == "c") {
				// continue execution until breakpoint is hit or end is reached
				unsigned pc = cpu_->registers_->getPC();
				do {
					Step();
					pc = cpu_->registers_->getPC();
				} while (std::find(breakpoint_list.begin(), breakpoint_list.end(), pc) == breakpoint_list.end());
				printf("Breakpoint found at 0x%04x\n", (unsigned)cpu_->registers_->getPC());
			}
			else if (cmd == "help" || cmd == "h") {
				// print help
				Logger::DebugHelp();
			}
			else if (cmd == "restart" || cmd == "r") {
				// run or restart execution
				cpu_->Reset();
			}
			else if (cmd == "quit" || cmd == "q") {
				// quit debugger
				is_running_ = false;
			}
			else {
				continue;
			}
		}
	}
}

void Emulator::NoDebug()
{
	Logger::Disable();
}
