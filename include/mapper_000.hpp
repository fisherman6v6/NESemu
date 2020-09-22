#pragma once
#include"i_mapper.hpp"
#include <fstream>
#include <memory>
#include <iostream>

/*All Banks are fixed,

CPU $6000-$7FFF: Family Basic only: PRG RAM, mirrored as necessary to fill entire 8 KiB window, write protectable with an external switch
CPU $8000-$BFFF: First 16 KB of ROM.
CPU $C000-$FFFF: Last 16 KB of ROM (NROM-256) or mirror of $8000-$BFFF (NROM-128).

*/

constexpr auto PRG_ROM_BLOCK_SIZE = 16384;
constexpr auto CHR_ROM_BLOCK_SIZE = 8192;

constexpr auto PRG_ROM_BASE_ADDR_1 = 0x8000;
constexpr auto PRG_ROM_END_ADDR_1 = 0xbfff;
constexpr auto PRG_ROM_BASE_ADDR_2 = 0xc000;
constexpr auto PRG_ROM_END_ADDR_2 = 0xffff;
constexpr auto CHR_ROM_BASE_ADDR = 0;
constexpr auto CHR_ROM_END_ADDR = 0x1fff;




class Mapper_000 : public IMapper
{
public:
	Mapper_000(std::istream& rom, INesFlags& ines_flags);

	~Mapper_000();

	uint8_t ReadByte(uint16_t address) const override;
	bool WriteByte(uint16_t address, uint8_t value) override;

private:

	//std::unique_ptr<uint8_t[]> prg_rom_;	// 16 KiB for NROM-128, 32 KiB for NROM-256
	//std::unique_ptr<uint8_t[]> chr_rom_;	// 8 KiB ROM (DIP-28 standard pinout) but most emulators support RAM
	//std::unique_ptr<uint8_t[]> prg_ram_;	// 2 or 4 KiB, not bankswitched, only in Family Basic (but most emulators provide 8)

	uint8_t* prg_rom_;	// 16 KiB for NROM-128, 32 KiB for NROM-256
	uint8_t* chr_rom_;	// 8 KiB ROM (DIP-28 standard pinout) but most emulators support RAM
	//uint8_t prg_rom_[PRG_ROM_BLOCK_SIZE];
	//uint8_t chr_rom_[CHR_ROM_BLOCK_SIZE];

	bool nrom_type;						// 0 for NROM128, 1 for NROM256
};

