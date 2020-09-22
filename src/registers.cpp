#include "registers.hpp"

Registers::Registers() :	
	a_(0x00), 
	x_(0x00), 
	y_(0x00), 
	s_(0xfd),
	p_(0x24),
	pc_(0xc000)
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
	pc_ = 0xc000;
}
