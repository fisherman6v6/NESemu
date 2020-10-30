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

}

void Emulator::NoDebug()
{
	Logger::Enable();
	Logger::Log("Starting emulator in Run Mode");
	Logger::Log("Not Yet implemented...exit");
	return;
}
