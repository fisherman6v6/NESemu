#pragma once
#include "i_memory_unit.hpp"
#include "pch.hpp"
#include <string>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>
#include "i_mapper.hpp"
#include "mapper_000.hpp"

/*
+++++++++++++++++++++++++INES header++++++++++++++++++++++++++++++++++++++

	Here is the INES data format taken from nesdevwiki:

header[0, 3] = "NES\x1a" the header string identifying a INES ROM. the 0x1A
			   is the MS-DOS end of line terminator.

header[4] = The number of PRG-ROM inside the file (0x4000 * header[4])
			bytes.

header[5] = The number of CHR-ROM inside the file (0x2000 * header[5])
			bytes.

header[6] =
76543210
||||||||
||||+||+- 0xx0: horizontal mirroring; 0xx1: vertical mirroring;
		  1xxx: four-screen mirroring
|||| |+-- 1: SRAM in CPU $6000-$7FFF, if present, is battery backed
|||| +--- 1: 512-byte trainer at $7000-$71FF (stored before PRG data)
++++----- Lower nybble of mapper number

header[7] =
76543210
||||  ||
||||  |+- VS Unisystem
||||  +-- PlayChoice-10 (8KB of Hint Screen data stored after CHR data)
++++----- Upper nybble of mapper number

header[8] = Size of PRG RAM in 8 KB units
			(Value 0 implies 8 KB for compatibility; see PRG RAM circuit)

header[9] =
76543210
||||||||
|||||||+- TV system (0: NTSC; 1: PAL)
+++++++-- Reserved, set to zero
Not alot of emulator adheres to this, and many if not all
ROMs dumped do not use this flag.

header[10] =
76543210
  ||  ||
  ||  ++- TV system (0: NTSC; 2: PAL; 1/3: dual compatible)
  |+----- SRAM in CPU $6000-$7FFF is 0: present; 1: not present
  +------ 0: Board has no bus conflicts; 1: Board has bus conflicts
header[10] is unofficial from the INES spec, if you want to adhere
to the spec, treat header[10] as unused (zero-filled).

header[11,15] are unused (zero-filled).

	After this, there is a 512 bytes trainer if bit 2 of header 6 is set,
and then header[4] * 16384 bytes of PRG-ROM,
then header[5] * 8192 of CHR-ROM, and 8192 bytes of PlayChoice-10 data
at after the CHR-ROM.
*/

constexpr auto HEADER_LENGHT = 16;

class Cartridge : IMemoryUnit
{
public:
	Cartridge(const std::string& path);
	~Cartridge();

	uint8_t ReadByte(uint16_t address) const override;
	bool WriteByte(uint16_t address, uint8_t value) override;

	bool LoadRom(const std::string& path);

private:

	std::unique_ptr<IMapper> mapper_;

	bool ParseHeader(std::istream& file);

	char header_[HEADER_LENGHT];
	

};

