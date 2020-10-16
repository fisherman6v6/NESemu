#pragma once
#include <string>
#include <fstream>
#include <memory>
#include "i_memory_unit.hpp"
#include "logger.hpp"
#include "cartridge.hpp"
#include "ppu.hpp"
#include "cstring"

/* CPU Memory Map
--------------------------------------- $10000
 Upper Bank of Cartridge ROM
--------------------------------------- $C000
 Lower Bank of Cartridge ROM
--------------------------------------- $8000
 Cartridge RAM (may be battery-backed)
--------------------------------------- $6000
 Expansion Modules
--------------------------------------- $5000
 Input/Output
--------------------------------------- $2000
 2kB Internal RAM, mirrored 4 times
--------------------------------------- $0000
*/

constexpr auto iRAM_BASE = 0x0000;	// internal ram 2kB
constexpr auto iRAM_END = 0x07ff;
constexpr auto iRAM_MIRROR_END = 0x1fff;
constexpr auto RAM_SIZE = 0x0800;

constexpr auto cROM_BASE = 0x8000;	// cartiridge rom
constexpr auto cROM_END = 0xffff;
constexpr auto cROM_SIZE = 0x8000;

constexpr auto cRAM_BASE = 0x6000;	// cartridge ram
constexpr auto cRAM_END = 0x7fff;
constexpr auto cRAM_SIZE = 0x8000;

constexpr auto STACK_BASE = 0x0100;
constexpr auto STACK_END = 0x01ff;

constexpr auto PPU_REGS_BASE = 0x2000;
constexpr auto PPU_REGS_END = 0x2007;
constexpr auto PPU_REGS_MIRROR_END = 0x3fff;
constexpr auto PPU_REGS_SIZE = 8;

constexpr auto APU_REGS_BASE = 0x4000;	// NES APU and I/O registers
constexpr auto APU_REGS_END = 0x4017;

constexpr auto CARTRIDGE_BASE = 0x4020; // Cartridge space: PRG ROM, PRG RAM, and mapper registers
constexpr auto CARTRIDGE_END = 0xffff;

// 2 bytes interrupt vectors
constexpr auto NMI_BASE = 0xfffa; 
constexpr auto RESET_BASE = 0xfffc;
constexpr auto IRQ_BASE = 0x0fffe;

/*
	0x0000 - 0x00ff : Zero Page
	0x0100 - 0x01ff : Stack
	0xfffa - 0xfffb : non-maskable interrupt handler
	0xfffc - 0xfffd : BRK request handler
	0xfffe - 0xffff : Interrupt request handler

*/

class Ppu;

class Mmu : IMemoryUnit
{
public:
	Mmu();
	~Mmu();

	void Init(Cartridge* cartridge, Ppu* ppu);

	uint8_t ReadByte(uint16_t address) const override;
	bool WriteByte(uint16_t address, uint8_t value) override;

	uint16_t ReadWord(uint16_t address) const;
	bool WriteWord(uint16_t address, uint16_t value);

private:
	uint8_t iram_[RAM_SIZE];
	Cartridge* cartridge_;
	Ppu* ppu_;
};

