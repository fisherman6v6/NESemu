#include "mapper_000.hpp"

Mapper_000::Mapper_000(std::istream& rom, INesFlags& ines_flags)
{
	if (ines_flags.prg_rom_num_ > 2) {
		// Wrong mapper
		Logger::LogError("Mapper Error");
		exit(EXIT_FAILURE);
	}

	/*
	ines_flags.prg_rom_num_ == 1 -> NROM128
	ines_flags.prg_rom_num_ == 2 -> NROM256
	*/

	ines_flags.prg_rom_num_ == 1 ? nrom_type = 0 : nrom_type = 1;

	//prg_rom_ = std::make_unique<uint8_t>(new uint8_t[PRG_ROM_BLOCK_SIZE * ines_flags.prg_rom_num_]);
	//chr_rom_ = std::make_unique<uint8_t>(new uint8_t[CHR_ROM_BLOCK_SIZE);
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
	
	for (auto i = 0; i < PRG_ROM_BLOCK_SIZE * ines_flags.prg_rom_num_; i++) {
		// read PROG ROM
		rom.get(reinterpret_cast<char*>(prg_rom_)[i]);
	}

	for (int i = 0; i < CHR_ROM_BLOCK_SIZE; i++) {
		// read CHR ROM
		rom.get(reinterpret_cast<char*>(chr_rom_)[i]);
	}

	/*uint8_t test_prg[2*PRG_ROM_BLOCK_SIZE];
	//uint8_t test_chr[CHR_ROM_BLOCK_SIZE];

	memcpy(test_prg, prg_rom_, PRG_ROM_BLOCK_SIZE);
	//memcpy(test_chr, chr_rom_, CHR_ROM_BLOCK_SIZE);

	std::ofstream file;
	file.open("prg.txt", std::ostream::out);

	for (auto i = 0; i < 2*PRG_ROM_BLOCK_SIZE; i++) {
		file << std::hex << (unsigned)test_prg[i] << "\n";
	}*/

	//rom.read(reinterpret_cast<char*>(prg_rom_.get()), PRG_ROM_BLOCK_SIZE * ines_flags.prg_rom_num_);
	//rom.read(reinterpret_cast<char*>(prg_rom_), PRG_ROM_BLOCK_SIZE * ines_flags.prg_rom_num_);

	//chr_rom_ = std::make_unique<uint8_t[]>(CHR_ROM_BLOCK_SIZE);

	//reading CHR ROM, always one block in NROM
	//rom.read(reinterpret_cast<char*>(prg_rom_.get()), CHR_ROM_BLOCK_SIZE);
	//rom.read(reinterpret_cast<char*>(prg_rom_), CHR_ROM_BLOCK_SIZE);


}

Mapper_000::~Mapper_000()
{
	Logger::Log("Mapper destructor called");
	delete[] prg_rom_;
	delete[] chr_rom_;
}

uint8_t Mapper_000::ReadByte(uint16_t address) const
{
	if (address >= PRG_ROM_BASE_ADDR_0 && address <= PRG_ROM_END_ADDR_0) {
		return prg_rom_[address - PRG_ROM_BASE_ADDR_0];
	}
	else if (address >= PRG_ROM_BASE_ADDR_1 && address <= PRG_ROM_END_ADDR_1) {
		if (nrom_type) {
			//NROM256
			return prg_rom_[address - PRG_ROM_BASE_ADDR_0];
		}
		else {
            //NROM128 -> Mirroring of 0x8000 - 0xbfff
			return prg_rom_[address - PRG_ROM_BASE_ADDR_1];
		}
	}
	/* CHR ROM reading. This rom is read by the PPU. Cpu cannot access it */
	else if (address >= CHR_ROM_BASE_ADDR && address <= CHR_ROM_END_ADDR) {
		/*
		$0000-$0FFF Pattern table 0 
		$1000-$1FFF Pattern table 1 
		*/
		return chr_rom_[address - CHR_ROM_BASE_ADDR];
	} 

	Logger::LogError("Wrong Address %u", address);

	exit(EXIT_FAILURE);
}

bool Mapper_000::WriteByte(uint16_t address, uint8_t value)
{
	Logger::LogError("Can't write to ROM !");
	return true;
}
