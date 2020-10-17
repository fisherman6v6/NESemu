#include "cartridge.hpp"

Cartridge::Cartridge(const std::string& path)
{
	LoadRom(path);
}

Cartridge::~Cartridge() {
	Logger::Log("Cart destructor called");
}

uint8_t Cartridge::ReadByte(uint16_t address) const
{
	return mapper_->ReadByte(address);
}

bool Cartridge::WriteByte(uint16_t address, uint8_t value)
{
	return true;
}

bool Cartridge::LoadRom(const std::string & path)
{
	std::ifstream rom(path, std::ios::in | std::ios::binary);

	if (rom) {

		// get length of file:
		/*rom.seekg(0, rom.end);
		int length = rom.tellg();
		rom.seekg(0, rom.beg);*/

		ParseHeader(rom);

		return false;
	}
	Logger::LogError("File reading Error");
	exit(EXIT_FAILURE);
}

bool Cartridge::ParseHeader(std::istream & file)
{
	file.read(header_, HEADER_LENGHT);

	// checking if iNES format
	if (!(header_[0] == 'N' && header_[1] == 'E' && header_[2] == 'S' && header_[3] == 0x1A)) {
		Logger::LogError("Input file format unknown");
		return true;
	}
	if ((header_[7] & 0x0C) == 0x08) {
		// NES2.0 format, to be implemented
	}

	INesFlags ines_flags;

	ines_flags.prg_rom_num_ = header_[4];
	ines_flags.chr_rom_num_ = header_[5];
	ines_flags.chr_ram_num_ = header_[8];
	ines_flags.mirroring_ = CheckBit((uint8_t)header_[6], 0);
	ines_flags.vs_system_ = CheckBit((uint8_t)header_[7], 0);
	ines_flags.bb_prg_ram_ = CheckBit((uint8_t)header_[6], 1);
	ines_flags.trainer_ = CheckBit((uint8_t)header_[6], 2);
	ines_flags.mirroring_4_ = CheckBit((uint8_t)header_[6], 3);
	ines_flags.ntsc_ = CheckBit((uint8_t)header_[9], 0);

	// Lower nibble
	uint8_t mapper_lo = (header_[6] & 0xf0) >> 4;
	// Higher nibble
	uint8_t mapper_hi = header_[7] & 0xf0;
	// iNES mapper number
	uint8_t mapper = mapper_hi | mapper_lo; 

	switch (mapper)
	{
	case 0:
		mapper_ = std::make_unique<Mapper_000>(file, ines_flags);
		break;
	default:
		return true;
	}

	return false;
}
