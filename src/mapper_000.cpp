#include "mapper_000.hpp"

Mapper_000::Mapper_000(std::istream& rom, INesFlags& ines_flags)
{
	if (ines_flags.prg_rom_num_ > 2) {
		// Wrong mapper
		std::cout << "Mapper Error" << std::endl;
		exit(EXIT_FAILURE);
	}

	ines_flags.chr_rom_num_ == 1 ? nrom_type = 0 : nrom_type = 1;

	// ines_flags.prg_rom_num_ == 1 -> NROM128
	// ines_flags.prg_rom_num_ == 2 -> NROM256
	//prg_rom_ = std::make_unique<uint8_t[]>(PRG_ROM_BLOCK_SIZE * ines_flags.prg_rom_num_);
	prg_rom_ = new uint8_t[PRG_ROM_BLOCK_SIZE * ines_flags.prg_rom_num_];
	chr_rom_ = new uint8_t[CHR_ROM_BLOCK_SIZE];

	char c;

	// skip trainer
	if (ines_flags.trainer_) {
		for (auto i = 0; i < 512; i++) {
			// trainer
			rom.get(c);
		}
	}
	

	for (int i = 0; i < PRG_ROM_BLOCK_SIZE * ines_flags.prg_rom_num_; i++) {
		// read PROG ROM
		rom.get(reinterpret_cast<char*>(prg_rom_)[i]);
	}

	for (int i = 0; i < CHR_ROM_BLOCK_SIZE; i++) {
		// read CHR ROM
		rom.get(reinterpret_cast<char*>(chr_rom_)[i]);
	}

	// reading PRG ROM
	//rom.seekg(addr, rom.beg);

	//rom.read(reinterpret_cast<char*>(prg_rom_.get()), PRG_ROM_BLOCK_SIZE * ines_flags.prg_rom_num_);
	//rom.read(reinterpret_cast<char*>(prg_rom_), PRG_ROM_BLOCK_SIZE * ines_flags.prg_rom_num_);

	//chr_rom_ = std::make_unique<uint8_t[]>(CHR_ROM_BLOCK_SIZE);

	//reading CHR ROM, always one block in NROM
	//rom.read(reinterpret_cast<char*>(prg_rom_.get()), CHR_ROM_BLOCK_SIZE);
	//rom.read(reinterpret_cast<char*>(prg_rom_), CHR_ROM_BLOCK_SIZE);


}

Mapper_000::~Mapper_000()
{
	delete[] prg_rom_;
	delete[] chr_rom_;
}

uint8_t Mapper_000::ReadByte(uint16_t address) const
{
	if (address >= PRG_ROM_BASE_ADDR_1 && address <= PRG_ROM_END_ADDR_1) {
		return prg_rom_[address - PRG_ROM_BASE_ADDR_1];
	}
	else if (address >= PRG_ROM_BASE_ADDR_2 && address <= PRG_ROM_END_ADDR_2) {
		if (nrom_type) {
			//NROM256
			return prg_rom_[address + PRG_ROM_BASE_ADDR_1 - PRG_ROM_BASE_ADDR_2];
		}
		else {
			return prg_rom_[address - PRG_ROM_BASE_ADDR_2];
		}
	}

	// CHR ROM READING MISSING

	std::cout << "Wrong Address " << address << std::endl;
	exit(EXIT_FAILURE);
}

bool Mapper_000::WriteByte(uint16_t address, uint8_t value)
{
	std::cout << "Can't write to ROM !" << std::endl;
	return true;
}
