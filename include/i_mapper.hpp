#pragma once
#include "i_memory_unit.hpp"

struct INesFlags {
	size_t prg_rom_num_;	// number of program ROM
	size_t chr_rom_num_;	// number of chr ROM, or VROM
	size_t chr_ram_num_;	// Number of 8kB RAM banks. For compatibility with the previous versions of the .NES format, assume 1x8kB RAM page when this byte is zero.
	bool mirroring_;		// 1 for vertical mirroring, 0 for horizontal mirroring.
	bool mirroring_4_;		// four screen mirroring
	bool vs_system_;		// 1 for VS-System cartridges.
	bool bb_prg_ram_;		// 1 for battery-backed PROG RAM at $6000-$7FFF. Size in 8kB units
	bool trainer_;			// 1 for a 512-byte trainer at $7000-$71FF.
	bool ntsc_;				// 1 for PAL cartridges, otherwise assume NTSC.
};

class IMapper : public IMemoryUnit
{
public:
	virtual ~IMapper() {};
};

