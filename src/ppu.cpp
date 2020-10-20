#include "ppu.hpp"

const uint8_t colors_[]
{
    0xEB,   //off
    0xC4,   //33% on   
    0x60,   //66% on
    0x00    //on
};



Ppu::Ppu() :
            ppu_cycles_(21),
            odd_frame_(false), 
            ppuctrl_(0x00),
            ppumask_(0x00),
            ppustatus_(0xa0),
            oamaddr_(0x00),
            ppuscroll_(0x00),
            ppuscroll_latch_(false),
            ppuaddr_(0x00),
            ppuaddr_latch_(false),
            ppudata_(0x00)          
{

}

Ppu::~Ppu() {
    Logger::Log("Ppu destructor called");
}

void Ppu::Init(Cpu* cpu, Cartridge* cartridge) {
    this->cpu_ = cpu;
    this->cartridge_ = cartridge;
}

void Ppu::Step(unsigned cycles) {
    ppu_cycles_ += cycles * 3;
    RenderPatternTable();
}

void Ppu::RenderPatternTable() {
    /*
	$0000-$0FFF Pattern table 0 - left
	$1000-$1FFF Pattern table 1 - right
	*/
    uint8_t left[PATTERN_TABLE_SIZE];
    //uint8_t right[PATTERN_TABLE_SIZE];

    /* Fill pattern tables*/

    for (auto i = 0; i < PATTERN_TABLE_SIZE; i++) {
        left[i] = cartridge_->ReadByte(i);
    }

    /*for (auto i = 0; i < PATTERN_TABLE_SIZE; i++) {
        right[i] = cartridge_->ReadByte(i + PATTERN1_BASE);
    }*/

    uint8_t tile[LINES_PER_TILE] = {0};
    unsigned pixel_pattern[PIXEL_LINES_PER_PP][PIXEL_PER_LINE] = {0};

    for (auto i = 0; i < TILES_PER_PT; i++) {

        /* Fill a tile */
        for (auto j = 0; j < LINES_PER_TILE; j++) {
            tile[j] = left[i * LINES_PER_TILE + j];
        }

        /* Fill pixel pattern */
        for (auto k = 0; k < PIXEL_LINES_PER_PP; k++) {
            
            for (auto l = 0; l < PIXEL_PER_LINE; l++) {
                
                /* case 0 - 0 */
                if (!CheckBit(tile[k + LINES_PER_PLANE], (PIXEL_PER_LINE - 1 - l)) && !CheckBit(tile[k], (PIXEL_PER_LINE - 1 - l))) {
                    pixel_pattern[k][l] = 0;
                }
                /* case 0 - 1 */
                else if (!CheckBit(tile[k + LINES_PER_PLANE], (PIXEL_PER_LINE - 1 - l)) && CheckBit(tile[k], (PIXEL_PER_LINE - 1 - l))) {
                    pixel_pattern[k][l] = 1;
                }
                /* case 1 - 0 */
                else if (CheckBit(tile[k + LINES_PER_PLANE], (PIXEL_PER_LINE - 1 - l)) && !CheckBit(tile[k], (PIXEL_PER_LINE - 1 - l))) {
                    pixel_pattern[k][l] = 2;
                }
                /* case 1 - 1 */
                else {
                    pixel_pattern[k][l] = 3;
                }
            }
        }

        /* copy pixel pattern into display*/
        for(auto j = 0; j < PIXEL_LINES_PER_PP; j++) {
            for (auto k = 0; k < PIXEL_PER_LINE; k++) {
                display_[i][j][k] = colors_[pixel_pattern[j][k]];
            }
        }
    }
}

void Ppu::Reset() {
    ppuctrl_ = 0x00;
    ppumask_ = 0x00;
    ppustatus_ &= 0x80;
/*  oamaddr_ unchanged
    ppuaddr_ unchanged
*/
    ppuscroll_ = 0x00;
    ppudata_ = 0x00;
    odd_frame_ = false;
}

uint8_t Ppu::ReadByte(uint16_t address) const {
    switch (address)
    {
    case PPUSTATUS_ADDR :
        return ppustatus_;
    case OAMDATA_ADDR :
        return oamdata_;
    case PPUDATA_ADDR :
        return ppudata_;
    default:
        Logger::LogError("PPU: Can't read from this address: 0x%04x", address);;
        return 0;
    }
    return 0;
}

bool Ppu::WriteByte(uint16_t address, uint8_t value) {
    switch (address)
    {
    case PPUCTRL_ADDR :
        ppuctrl_ = value;
        return false;
    case PPUMASK_ADDR :
        ppumask_ = value;
        return false;
    case OAMADDR_ADDR :
        oamaddr_ = value;
        return false;
    case OAMDATA_ADDR :
        oamdata_ = value;
        return false;
    case PPUSCROLL_ADDR : {
        if (!ppuscroll_latch_) {
            /* First write
                X scroll*/
            ppuscroll_ = (uint16_t)value << 8;
        }
        else {
            /* Second write
                Y scroll*/
            ppuscroll_ |= (uint16_t)value;
            
        }
        ppuscroll_latch_ = !ppuscroll_latch_;
        return false;
    }
    case PPUADDR_ADDR : {
        if (!ppuaddr_latch_) {
            /* First write
                most significant byte*/
            ppuaddr_ = (uint16_t)value << 8;
        }
        else {
            /* Second write
                least significant byte*/
            ppuaddr_ |= (uint16_t)value;
        }
        ppuaddr_latch_ = !ppuaddr_latch_;
        return false;
    }
    case PPUDATA_ADDR :
        ppudata_ = value;
        return false;
    case OAMDMA_ADDR:
        oamdma_ = value;
        return false;
    default:
        Logger::LogError("PPU: Can't write to this address: 0x%04x", address);
        return true;
    }
    return 0;
}