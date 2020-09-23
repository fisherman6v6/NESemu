#pragma once
#include "pch.hpp"

/*Memory unit interface. Defines read and write bytes, to be implemented in derived classes*/

class IMemoryUnit
{	
public:
	virtual ~IMemoryUnit() {};
	virtual uint8_t ReadByte(uint16_t address) const = 0;
	virtual bool WriteByte(uint16_t address, uint8_t value) = 0;

	//virtual uint16_t ReadWord(uint16_t address) const = 0;
	//virtual bool WriteWord(uint16_t address, uint16_t value) = 0;

};

