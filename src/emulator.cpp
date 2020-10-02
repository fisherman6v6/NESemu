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
	cpu_->LoadRom(path);
	cpu_->Reset();
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
	Logger::Log("Starting emulator in Debug Mode");

	std::list<unsigned> breakpoint_list;
	std::string line;

	while (is_running_) {

		Logger::Log("Next instruction at PC = 0x%04x", (unsigned)cpu_->registers_->getPC());

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
					Logger::Log("Breakpoint set at 0x%04x", bline);
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
							Logger::Log("No brekpoint set");
						}
						else {
							for (auto it = breakpoint_list.begin(); it != breakpoint_list.end(); it++, cnt++) {
								//std::cout << "b" << cnt <<" : " << std::hex << *it << "\n";
								Logger::Log("b %d : 0x04%x", cnt, *it);
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
				Logger::Log("Breakpoint found at 0x%04x", (unsigned)cpu_->registers_->getPC());
			}
			else if (cmd == "help" || cmd == "h") {
				// print help
				Logger::DebugHelp();
			}
			else if (cmd == "restart" || cmd == "r") {
				// run or restart execution
				cpu_->Reset();
			}
			else if (cmd == "d" || cmd == "delete") {
				// delete breakpoint
				unsigned b_num;
				stream >> b_num;
				unsigned pc = cpu_->registers_->getPC();
				breakpoint_list.remove(pc);
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
	Logger::Disable();
}

void Emulator::NoDebug()
{
	Logger::Disable();
}
