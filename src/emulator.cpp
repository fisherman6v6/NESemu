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
			char c;
			stream >> c;

			switch (c)
			{
			case 's':
				// step
				Step();
				break;
			case 'b': {
				unsigned bline;
				if (stream >> bline) {
					breakpoint_list.push_back(bline);
					printf("Breakpoint set at 0x%04x\n", bline);
				}
				break;
			}
			case 'p': {
				// print
				std::string to_print;
				if (stream >> to_print) {
					if (to_print == "regs") {
						Logger::LogCpuRegisters(cpu_->registers_);
					}
					else if (to_print == "mem") {
						unsigned start, size;
						if (stream >> start >> size) {
							Logger::LogMemory(cpu_->mmu_, start, size);
						}
					}
				}
				break;
			}
			case 'c': {
				// continue
				unsigned pc = cpu_->registers_->getPC();
				do {
					Step();
					pc = cpu_->registers_->getPC();
				} while (std::find(breakpoint_list.begin(), breakpoint_list.end(), pc) == breakpoint_list.end());
				printf("Breakpoint found at 0x%04x\n", (unsigned)cpu_->registers_->getPC());
				break;
			}
			case 'h':
				// help
				Logger::DebugHelp();
				break;
			case 'r':
				// restart
				cpu_->Reset();
				break;
			case 'q':
				is_running_ = false;
				break;

			default:
				break;
			}
		}

	}

	


	
}

void Emulator::NoDebug()
{
	Logger::Disable();
}
