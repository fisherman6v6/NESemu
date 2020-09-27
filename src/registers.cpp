#include "registers.hpp"

#define STARTING_ADDR 0xFFFC

Registers::Registers() :	
	a_(0x00), 
	x_(0x00), 
	y_(0x00), 
	s_(0xfd),
	p_(0x24),
	pc_(STARTING_ADDR)
{
}

Registers::~Registers()
{
}

void Registers::Reset()
{
	a_ = 0x00;
	x_ = 0x00;
	y_ = 0x00;
	s_ = 0xfd;
	p_ = 0x24;
	pc_ = STARTING_ADDR;
}
