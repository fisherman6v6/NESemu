#include "mmu.hpp"

Mmu::Mmu()
{
	memset(iram_, 0x00, RAM_SIZE);
}

Mmu::~Mmu()
{
}

uint8_t Mmu::ReadByte(uint16_t address) const
{
	if (address >= iRAM_BASE && address <= iRAM_END) {
		return iram_[address];
	}

	else if (address >= cROM_BASE && address <= cROM_END) {
		return cartridge_->ReadByte(address);
	}
	
	else {
		std::cout << "Address not yet mapped" << std::endl;
		exit(EXIT_FAILURE);
	}
}

bool Mmu::WriteByte(uint16_t address, uint8_t value)
{
	if (address >= iRAM_BASE && address <= iRAM_END) {
		iram_[address] = value;
		return 0;
	}

	return 1;

}

uint16_t Mmu::ReadWord(uint16_t address) const
{
	// Little endian
	uint16_t word_lo = (uint16_t)ReadByte(address);
	uint16_t word_hi = (uint16_t)ReadByte(address + 1);
	uint16_t word = (word_hi << 8 ) | word_lo;
	return word;
}

bool Mmu::WriteWord(uint16_t address, uint16_t value)
{
	WriteByte(address, (uint8_t)(value & 0x00ff));
	WriteByte(address + 1, (uint8_t)(value >> 8));
	return 0;
}

bool Mmu::LoadRom(const std::string & path)
{
	cartridge_ = std::make_unique<Cartridge>(path);
	return false;
}
