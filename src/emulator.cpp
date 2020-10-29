#include "emulator.hpp"

Emulator::Emulator()
{

}

Emulator::Emulator(bool mode, const std::string& path, const std::string& logfile_path) {

	this->cpu_ = new Cpu;
	this->ppu_ = new Ppu;
	this->mmu_ = new Mmu;
	this->cartridge_ = new Cartridge(path);
	this->logfile_path_ = logfile_path;

	cpu_->Init(mmu_, ppu_);
	ppu_->Init(cpu_, cartridge_);
	mmu_->Init(cartridge_, ppu_);

	mode ? RunCallback = &Emulator::Debug : RunCallback = &Emulator::NoDebug;
	is_running_ = false;

	cpu_->Reset();
}

Emulator::~Emulator() {

	Logger::Log("Emu destructor called");
	delete cartridge_;
	delete mmu_;
	delete ppu_;
	delete cpu_;
}

void Emulator::Step()
{
	cpu_->Step();
}

void Emulator::Run() {

	is_running_ = true;

	if (RunCallback != nullptr) {
		(this->*RunCallback)();
	}
}

void Emulator::Reset() {
	cpu_->Reset();
	ppu_->Reset();
}

void Emulator::Debug() {

	Logger::Enable();
	Logger::Log("Starting emulator in Debug Mode");

	std::unique_ptr<Debugger> debugger = std::make_unique<Debugger>(cpu_, ppu_, mmu_, cartridge_, logfile_path_);
	debugger->Debug();


/* 	std::list<std::pair<unsigned, unsigned>> breakpoint_list;
	unsigned b_num = 0;
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
					breakpoint_list.push_back(std::make_pair(b_num, bline));
					b_num++;
					Logger::Log("Breakpoint set at 0x%04x", bline);
				}
			}
			else if (cmd == "print" || cmd == "p") {
				// print
				std::string to_print;
				if (stream >> to_print) {
					if (to_print == "regs") {
						// print cpu registers
						DebugLogger::LogCpuRegisters(cpu_->registers_);
					}
					else if (to_print == "mem") {
						// print memory 
						unsigned start, size;
						if (stream >> start) {
							if (stream >> size) {
								DebugLogger::LogMemory(cpu_->mmu_, start, size);
							}
							else {
								DebugLogger::LogMemory(cpu_->mmu_, start, 1);
							}
						}
					}
					else if (to_print == "b") {
						// print breakpoint list
						if (breakpoint_list.empty()) {
							Logger::Log("No brekpoint set");
						}
						else {
							for (auto it = breakpoint_list.begin(); it != breakpoint_list.end(); it++) {
								Logger::Log("b %u : 0x%04x",it->first, it->second);
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
					
				} while (std::find_if(breakpoint_list.begin(), breakpoint_list.end(), 
						[&pc](const std::pair<unsigned, unsigned>& element) {return element.second == pc; }) == breakpoint_list.end());
				Logger::Log("Breakpoint found at 0x%04x", (unsigned)cpu_->registers_->getPC());
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
				stream >> to_delete;
				breakpoint_list.remove_if([&to_delete](const std::pair<unsigned, unsigned>& element) { return element.first == to_delete; });
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
	//Logger::Disable(); */
}

void Emulator::NoDebug()
{
	Logger::Enable();
	Logger::Log("Starting emulator in Run Mode");
	Logger::Log("Not Yet implemented...exit");
	return;
}
