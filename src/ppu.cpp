#include "ppu.hpp"

Ppu::Ppu() {

}

Ppu::~Ppu() {
    Logger::Log("Ppu destructor called");
}

void Ppu::Init(std::shared_ptr<Cpu> cpu, std::shared_ptr<Cartridge> cartridge) {
    this->cpu_ = cpu;
    this->cartridge_ = cartridge;
}

uint8_t Ppu::ReadByte(uint16_t address) const {
    return 0;
}

bool Ppu::WriteByte(uint16_t address, uint8_t value) {
    return false;
}