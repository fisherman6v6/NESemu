#pragma once
#include "pch.hpp"
#include "cpu.hpp"

constexpr auto PPUCTRL_ADDR = 0x2000;
constexpr auto PPUMASK_ADDR = 0x2001;
constexpr auto PPUSTATUS_ADDR = 0x2002;
constexpr auto OAMADDR_ADDR = 0x2003;
constexpr auto OAMDATA_ADDR = 0x2004;
constexpr auto PPUSCROLL_ADDR = 0x2005;
constexpr auto PPUADDR_ADDR = 0x2006;
constexpr auto PPUDATA_ADDR = 0x2007;
constexpr auto OAMDMA_ADDR = 0x4014;

constexpr auto OAM_SIZE = 256;
constexpr auto PALETTE_SIZE = 28;
constexpr auto VRAM_SIZE = 2048;


class Ppu {

public:
    Ppu();
    ~Ppu();

private:

uint8_t oam_[OAM_SIZE];
uint8_t palette_[PALETTE_SIZE];
uint8_t vram_[VRAM_SIZE];

/*PPU REGISTERS*/

/*bits: VPHB SINN 	
NMI enable (V), PPU master/slave (P), sprite height (H), 
background tile select (B), sprite tile select (S), increment mode (I), nametable select (NN) */
uint8_t ppuctrl_; 

/*bits: BGRs bMmG 	
color emphasis (BGR), sprite enable (s), background enable (b), sprite left column enable (M), 
background left column enable (m), greyscale (G) */
uint8_t ppumask_;

/*bits: VSO- ---- 	
vblank (V), sprite 0 hit (S), sprite overflow (O); read resets write pair for $2005/$2006 */
uint8_t ppustatus_;

/*bits: aaaa aaaa 	
OAM read/write address */
uint8_t oamaddr_;

/*bits: dddd dddd
OAM data read/write */
uint8_t oamdata_;

/*bits: xxxx xxxx
fine scroll position (two writes: X scroll, Y scroll) */
uint8_t ppuscroll_;

/*bits: aaaa aaaa 	
PPU read/write address (two writes: most significant byte, least significant byte)*/
uint8_t ppuaddr_;

/*bits: dddd dddd
PPU data read/write 
*/
uint8_t ppudata_;

/*bits: aaaa aaaa 
OAM DMA high address */
uint8_t oamdma_;
};