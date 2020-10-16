#include "mmu.hpp"

Mmu::Mmu()
{
	memset(iram_, 0x00, RAM_SIZE);
}

Mmu::~Mmu() {
	Logger::Log("Mmu destructor called");
};

void Mmu::Init(Cartridge* cartridge, Ppu* ppu) {
	this->cartridge_ = cartridge;
	this->ppu_ = ppu;
}

uint8_t Mmu::ReadByte(uint16_t address) const {
	if (address >= iRAM_BASE && address <= iRAM_MIRROR_END) {
		/*System memory at $0000-$07FF is mirrored at $0800-$0FFF, 
		$1000-$17FF, and $1800-$1FFF - attempting to access memory at, 
		for example, $0173 is the same as accessing memory at $0973, $1173, or $1973.*/
		return iram_[address % RAM_SIZE];
	}

	else if (address >= CARTRIDGE_BASE && address <= CARTRIDGE_END) {
		return cartridge_->ReadByte(address);
	}

	else if (address >= PPU_REGS_BASE && address <= PPU_REGS_MIRROR_END) {
		/* PPU regs are mirrored in every 8 bytes from $2008 through $3FFF, 
		so a write to $3456 is the same as a write to $2006. */
		address = (address % PPU_REGS_SIZE) + PPU_REGS_BASE;
		return ppu_->ReadByte(address);
	}
	
	else {
		Logger::LogError("Address not yet mapped");
		exit(EXIT_FAILURE);
	}
}

bool Mmu::WriteByte(uint16_t address, uint8_t value) {
	if (address >= iRAM_BASE && address <= iRAM_END) {
		iram_[address] = value;
		return false;
	}

	else if (address >= PPU_REGS_BASE && address <= PPU_REGS_MIRROR_END) {
		address = (address % PPU_REGS_SIZE) + PPU_REGS_BASE;
		return ppu_->WriteByte(address, value);
	}

	return true;

}

uint16_t Mmu::ReadWord(uint16_t address) const {
	// Little endian
	uint16_t word_lo = (uint16_t)ReadByte(address);
	uint16_t word_hi = (uint16_t)ReadByte(address + 1);
	uint16_t word = (word_hi << 8 ) | word_lo;
	return word;
}

bool Mmu::WriteWord(uint16_t address, uint16_t value) {
	WriteByte(address, (uint8_t)(value & 0x00ff));
	WriteByte(address + 1, (uint8_t)(value >> 8));
	return false;
}