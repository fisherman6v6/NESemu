#pragma once
#include "pch.hpp"
#include "cpu.hpp"
#include "cartridge.hpp"
#include "logger.hpp"
#include <SFML/Graphics.hpp>

/*The PPU addresses a 16kB space, $0000-3FFF, 
completely separate from the CPU's address bus. 
It is either directly accessed by the PPU itself, 
or via the CPU with memory mapped registers at $2006 and $2007. 
*/

/* PPU memory map*/

/* Registers */
constexpr auto PPUCTRL_ADDR = 0x2000;
constexpr auto PPUMASK_ADDR = 0x2001;
constexpr auto PPUSTATUS_ADDR = 0x2002;
constexpr auto OAMADDR_ADDR = 0x2003;
constexpr auto OAMDATA_ADDR = 0x2004;
constexpr auto PPUSCROLL_ADDR = 0x2005;
constexpr auto PPUADDR_ADDR = 0x2006;
constexpr auto PPUDATA_ADDR = 0x2007;
constexpr auto OAMDMA_ADDR = 0x4014;

/* Memory*/
constexpr auto PATTERN0_BASE = 0x0000;
constexpr auto PATTERN0_END = 0x0fff;
constexpr auto PATTERN1_BASE = 0x1000;
constexpr auto PATTERN1_END = 0x1fff;
constexpr auto NAMETABLE0_BASE = 0x2000;
constexpr auto NAMETABLE0_END = 0x23ff;
constexpr auto NAMETABLE1_BASE = 0x2400;
constexpr auto NAMETABLE1_END = 0x27ff;
constexpr auto NAMETABLE2_BASE = 0x2800;
constexpr auto NAMETABLE2_END = 0x2bff;
constexpr auto NAMETABLE3_BASE = 0x2c00;
constexpr auto NAMETABLE3_END = 0x2fff;
constexpr auto PALETTE_RAM_BASE = 0x3f00;
constexpr auto PALETTE_RAM_END = 0x3f1f;



constexpr auto OAM_SIZE = 256;
constexpr auto PALETTE_SIZE = 28;
constexpr auto VRAM_SIZE = 2048;
constexpr auto kPatterTableSize = 0x1000;

/* tiles per patter table*/
constexpr auto kTilesPerPatternTable = 128;
/* lines per tile */
constexpr auto kLinesPerTile = 16;
/* two planes in each tile*/
constexpr auto kLinesPerPlane = kLinesPerTile / 2; 

/* Pixel lines per pixel pattern*/
constexpr auto kPixelLinesPerPixelPattern = 8;

constexpr auto kPixelPerLine = 8;

#define WIN_L 128
#define WIN_H 256

class Cpu;
class Cartridge;

class Ppu : IMemoryUnit 
{
public:
    Ppu();
    ~Ppu();

    void Init(Cpu* cpu, Cartridge* cartridge);
    void Step(unsigned cycles);
    void Reset();

    uint8_t ReadByte(uint16_t address) const override;
	bool WriteByte(uint16_t address, uint8_t value) override;

private:

    /* Temporary: SFML rendering stuff*/

    uint8_t display_[WIN_H * WIN_L * 4];

    sf::RenderWindow window_;
    sf::Texture frame_texture_;
    sf::Sprite frame_sprite_;

    void Render();
    void HandleEvents();
    void CreateSprite();

    void RenderPatternTables();

    /**********************************/

    Cpu* cpu_;
    Cartridge* cartridge_;

    uint64_t ppu_cycles_;

    /* even/odd frame flag, toggled each frame 0 - even  1 - odd*/
    bool odd_frame_; 
    uint8_t oam_[OAM_SIZE];
    uint8_t palette_ram[PALETTE_SIZE];
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
    fine scroll position (two writes: X scroll, Y scroll) 
    latch is used to know if it's the first or second write*/
    uint16_t ppuscroll_;
    bool ppuscroll_latch_;

    /*bits: aaaa aaaa 	
    PPU read/write address (two writes: most significant byte, least significant byte)
    latch is used to know if it's the first or second write*/
    uint16_t ppuaddr_;
    bool ppuaddr_latch_;

    /*bits: dddd dddd
    PPU data read/write 
    */
    uint8_t ppudata_;

    /*bits: aaaa aaaa 
    OAM DMA high address */
    uint8_t oamdma_;
};