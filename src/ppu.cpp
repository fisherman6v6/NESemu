#include "ppu.hpp"

#define GRAPHICS 1
#define TEST 0

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
    //memset(display_, 0, WIN_H * WIN_L * 3);

    #if GRAPHICS
    window_.create(sf::VideoMode(WIN_L, WIN_H), "CHR");
    window_.setFramerateLimit(1000);
    window_.setVerticalSyncEnabled(false);
    window_.clear(sf::Color::Black);
    CreateSprite();
    #endif

}

Ppu::~Ppu() {
    Logger::Log("Ppu destructor called");
    window_.close();
}

void Ppu::Init(Cpu* cpu, Cartridge* cartridge) {
    this->cpu_ = cpu;
    this->cartridge_ = cartridge;
}

void Ppu::Step(unsigned cycles) {
    ppu_cycles_ += cycles * 3;
    #if GRAPHICS
    RenderPatternTable();
    Render();
    #endif
}

void Ppu::RenderPatternTable() {
    /*
	$0000-$0FFF Pattern table 0 - left
	$1000-$1FFF Pattern table 1 - right
	*/

    // uint8_t pattern_table[2 * kPatterTableSize];
    // uint8_t tile[kLinesPerTile] = {0};
    // uint8_t pixel_pattern[kPixelLinesPerPixelPattern][kPixelPerLine] = {0};

    // /* Fill pattern tables*/

    // for (auto i = 0; i < 2 * kPatterTableSize; i++) {
    //     pattern_table[i] = cartridge_->ReadByte(i);
    // }

    for (auto row = 0; row < WIN_H; row++) {

        for (auto col = 0; col < WIN_L; col++) {

            uint16_t addr = (row / 8 * 256) + (row % 8) + (col / 8) * 16;

            uint8_t pixel_msb = (uint8_t)CheckBit(cartridge_->ReadByte(addr), (7 - (col % 8)));
            uint8_t pixel_lsb = (uint8_t)CheckBit(cartridge_->ReadByte(addr + 8), (7 - (col % 8)));
            uint8_t pixel = 2*pixel_msb + pixel_lsb;

            display_[(row * WIN_L * 4) + (col * 4)] = colors_[pixel];
            display_[(row * WIN_L * 4) + (col * 4) + 1] = colors_[pixel];
            display_[(row * WIN_L * 4) + (col * 4) + 2] = colors_[pixel];
            display_[(row * WIN_L * 4) + (col * 4) + 3] = 0xff;

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

/* Temp: rendering functions*/
void Ppu::HandleEvents() {
    sf::Event event;
    while (window_.pollEvent(event)){
        if (event.type == sf::Event::Closed) {
            window_.close();
        }
        else if (event.type == sf::Event::KeyPressed) {
            (void)0;
        }
    }
}

void Ppu::Render() {

    window_.clear(sf::Color::Black);
    //sf::Image frame;
    //frame.create(WIN_L, WIN_H);

    /*for (auto row = 0; row < WIN_H; row++) {
        for (auto col = 0; col < WIN_L; col++) {
            frame.setPixel(row, col, sf::Color(display_[row*WIN_L + col], display_[row*WIN_L + col + 1], display_[row*WIN_L + col + 2]));
        }
    }*/

    window_.clear();
    //frame_texture_.update(frame);
    frame_texture_.update(display_);
    window_.draw(frame_sprite_);
    window_.display();

}

void Ppu::CreateSprite() {
    frame_texture_.create(WIN_L, WIN_H);

    const auto scale_x = static_cast<float>(window_.getSize().x) / WIN_L;
    const auto scale_y = static_cast<float>(window_.getSize().y) / WIN_H;

    frame_sprite_.setTexture(frame_texture_);
    frame_sprite_.setScale(scale_x, scale_y);
}
/****************************/