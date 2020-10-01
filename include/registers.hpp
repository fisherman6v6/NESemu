#pragma once
#include"pch.hpp"
// Flags
constexpr auto NEG = 7;	// Negative 1 == Neg
constexpr auto OVF = 6;	// Overflow 1 == 0vf
constexpr auto BRK = 4;	// BRK command	1 == BRK
constexpr auto DEM = 3;	// Decimal Mode 1 == Decimal
constexpr auto IRQ = 2;	// IRQ disable	1 == Disable
constexpr auto ZER = 1;	// Zero	1 == Result Zero
constexpr auto CAR = 0;	// Carry 1 == true

//Cpu can freely access registers. No need of getter and setter
class Cpu;

class Registers
{

private:
	uint8_t		a_;			// Accumulator
	uint8_t		x_;			// Index Register X
	uint8_t		y_;			// Index Register Y
	uint8_t		s_;			// Stack Pointer
	uint8_t		p_;			// Processor Status

	uint16_t	pc_;		//Program Counter

	friend class Cpu;
public:
	Registers();
	~Registers();

	uint8_t getA() const { return a_; };
	uint8_t getX() const { return x_; };
	uint8_t getY() const { return y_; };
	uint8_t getS() const { return s_; };
	uint8_t getP() const { return p_; };
	uint16_t getPC() const { return pc_; };

	bool CheckNeg() const { return CheckBit(p_, NEG); };
	bool CheckOvf() const { return CheckBit(p_, OVF); };
	bool CheckBrk() const { return CheckBit(p_, BRK); };
	bool CheckDec() const { return CheckBit(p_, DEM); };
	bool CheckIrd() const { return CheckBit(p_, IRQ); };
	bool CheckZer() const { return CheckBit(p_, ZER); };
	bool CheckCar() const { return CheckBit(p_, CAR); };

	void ClearNeg() { ClearBit(p_, NEG); };
	void ClearOvf() { ClearBit(p_, OVF); };
	void ClearBrk() { ClearBit(p_, BRK); };
	void ClearDec() { ClearBit(p_, DEM); };
	void ClearIrd() { ClearBit(p_, IRQ); };
	void ClearZer() { ClearBit(p_, ZER); };
	void ClearCar() { ClearBit(p_, CAR); };

	void SetNeg() { SetBit(p_, NEG); };
	void SetOvf() { SetBit(p_, OVF); };
	void SetBrk() { SetBit(p_, BRK); };
	void SetDec() { SetBit(p_, DEM); };
	void SetIrd() { SetBit(p_, IRQ); };
	void SetZer() { SetBit(p_, ZER); };
	void SetCar() { SetBit(p_, CAR); };
};




