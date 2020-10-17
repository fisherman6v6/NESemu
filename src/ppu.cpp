#include "ppu.hpp"

Ppu::Ppu() :
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
            ppuscroll_latch_ = true;
        }
        else {
            /* Second write
                Y scroll*/
            ppuscroll_ |= (uint16_t)value;
            ppuscroll_latch_ = false;
        }
        return false;
    }
    case PPUADDR_ADDR : {
        if (!ppuaddr_latch_) {
            /* First write
                most significant byte*/
            ppuaddr_ = (uint16_t)value << 8;
            ppuaddr_latch_ = true;
        }
        else {
            /* Second write
                least significant byte*/
            ppuaddr_ |= (uint16_t)value;
            ppuaddr_latch_ = false;
        }
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