#pragma once
#include <exception>
#include <string>
#include <iostream>
#include "logger.hpp"
#include "debug_logger.hpp"
#include "mmu.hpp"
#include "registers.hpp"

constexpr auto ISA_SIZE = 256;

class Emulator;

class Cpu
{
public:
	Cpu();
	~Cpu();
	
	uint64_t Step();

	void LoadRom(const std::string& rompath);
	
	void Reset();

private:

	friend class Emulator;

	std::unique_ptr<Mmu> mmu_;
	std::unique_ptr<Registers> registers_;
	uint64_t clock_cycles_;

					/* ISA */
	typedef uint8_t(Cpu::*Instruction)(uint8_t opcode);

	Instruction instructions_[ISA_SIZE]{};
	// Load-Store operations

	uint8_t LDA(uint8_t opcode);	// Load Accumulator
	uint8_t LDX(uint8_t opcode);	// Load X register
	uint8_t LDY(uint8_t opcode);	// Load Y register
	uint8_t STA(uint8_t opcode);	// Store Accumulator
	uint8_t STX(uint8_t opcode);	// Store X Register
	uint8_t STY(uint8_t opcode);	// Store Y Register

	// Register-Trasnfer operations

	uint8_t TAX(uint8_t opcode);	// Transfer accumulator to X
	uint8_t TAY(uint8_t opcode);	// Transfer accumulator to Y
	uint8_t TXA(uint8_t opcode);	// Transfer X to accumulator
	uint8_t TYA(uint8_t opcode);	// Transfer Y to accumulator

	// Stack Operations

	uint8_t TSX(uint8_t opcode);	// Transfer stack pointer to X
	uint8_t TXS(uint8_t opcode);	// Transfer X to stack pointer
	uint8_t PHA(uint8_t opcode);	// Push accumulator on stack
	uint8_t PHP(uint8_t opcode);    // Push processor status on stack   
	uint8_t PLA(uint8_t opcode);    // Pull accumulator from stack 
	uint8_t PLP(uint8_t opcode);    // Pull processor status from stack
	
	// Logical 

	uint8_t AND(uint8_t opcode);    // Logical AND
	uint8_t EOR(uint8_t opcode);    // Exclusive OR
	uint8_t ORA(uint8_t opcode);	// Logical Inclusive OR	
	uint8_t BIT(uint8_t opcode);    // Bit test 

	// Arithmetic

	uint8_t ADC(uint8_t opcode);    // Add with Carry
	uint8_t SBC(uint8_t opcode);    // Subtract with Carry    
	uint8_t CMP(uint8_t opcode);    // Compare accumulator
	uint8_t CPX(uint8_t opcode);    // Compare X register
	uint8_t CPY(uint8_t opcode);    // Compare Y register 

	// Increments and Decrements

	uint8_t INC(uint8_t opcode);    // Increment a memory location 
	uint8_t INX(uint8_t opcode);    // Increment the X register
	uint8_t INY(uint8_t opcode);    // Increment the Y register 
	uint8_t DEX(uint8_t opcode);    // Decrement a memory location
	uint8_t DEC(uint8_t opcode);    // Decrement the X register 
	uint8_t DEY(uint8_t opcode);    // Decrement the Y register
	
	// Shifts

	uint8_t ASL(uint8_t opcode);    // Arithmetic Shift Left
	uint8_t LSR(uint8_t opcode);    // Logical Shift Right
	uint8_t ROL(uint8_t opcode);    // Rotate Left
	uint8_t ROR(uint8_t opcode);    // Rotate Right

	// Jumps and Calls

	uint8_t JMP(uint8_t opcode);    // Jump to another location 
	uint8_t JSR(uint8_t opcode);    // Jump to a subroutine 
	uint8_t RTS(uint8_t opcode);    // Return from subroutine 
	
	// Branches
	
	uint8_t BCC(uint8_t opcode);    // Branch if carry flag clear 
	uint8_t BCS(uint8_t opcode);    // Branch if carry flag set 
	uint8_t BEQ(uint8_t opcode);    // Branch if zero flag set
	uint8_t BMI(uint8_t opcode);    // Branch if negative flag set 
	uint8_t BNE(uint8_t opcode);    // Branch if zero flag clear 
	uint8_t BPL(uint8_t opcode);    // Branch if negative flag clear
	uint8_t BVC(uint8_t opcode);    // Branch if overflow flag clear 
	uint8_t BVS(uint8_t opcode);    // Branch if overflow flag set

	// Status Flag Changes

	uint8_t CLC(uint8_t opcode);    // Clear carry flag 
	uint8_t CLD(uint8_t opcode);    // Clear decimal mode flag
	uint8_t CLI(uint8_t opcode);    // Clear interrupt disable flag 
	uint8_t CLV(uint8_t opcode);    // Clear overflow flag 
	uint8_t SEC(uint8_t opcode);    // Set carry flag
	uint8_t SED(uint8_t opcode);    // Set decimal mode flag 
	uint8_t SEI(uint8_t opcode);    // Set interrupt disable flag

	//System Functions

	uint8_t BRK(uint8_t opcode);    // Force an interrupt
	uint8_t NOP(uint8_t opcode);    // No Operation
	uint8_t RTI(uint8_t opcode);    // Return from Interrupt

				/*End ISA */

	// Addressing modes functions
	// function signature is the same on both cases even if the first group will always return 0. Just to be consistent


	// Page crossing can never occur
	inline bool Immediate(uint16_t& addr, int8_t& pc_inc);			// Immediate
	inline bool ZeroPage(uint16_t& addr, int8_t& pc_inc);			// Zero page
	inline bool ZeroPageX(uint16_t& addr, int8_t& pc_inc);			// Zero page,X
	inline bool ZeroPageY(uint16_t& addr, int8_t& pc_inc);			// Zero page,Y
	inline bool Absolute(uint16_t& addr, int8_t& pc_inc);			// Absolute
	inline bool IndexedIndirect(uint16_t& addr, int8_t& pc_inc);	// (Indirect,X)

	// Page crossing may occur
	inline bool AbsoluteX(uint16_t& addr, int8_t& pc_inc);			// Absolute,X
	inline bool AbsoluteY(uint16_t& addr, int8_t& pc_inc);			// Absolute,Y
	inline bool IndirectIndexed(uint16_t& addr, int8_t& pc_inc);	// (Indirect),Y
	
	// Only for branch op
	inline bool Relative(int8_t& pc_inc);
};

