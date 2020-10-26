#pragma once
#include <exception>
#include <string>
#include <iostream>
#include "logger.hpp"
#include "ppu.hpp"
#include "debug_logger.hpp"
#include "mmu.hpp"
#include "registers.hpp"


constexpr auto ISA_SIZE = 256;

class Emulator;
class Ppu;
class Mmu;

class Cpu
{
public:
	Cpu();
	~Cpu();

	void Init(Mmu* mmu, Ppu* ppu);
	
	uint64_t Step();
	
	void Reset();

	void SetIRQn() { irq_ = false; };
	void SetNMIn() { nmi_ = false; };

	void ClearIRQn() { irq_ = true; };
	void ClearNMIn() { nmi_ = true; };

	const Registers* GetRegisters() const { return registers_; }

private:

	friend class Emulator;

	Registers* registers_;
	Mmu* mmu_;
	Ppu* ppu_;
	
	uint64_t clock_cycles_;

	/* Interrupt pins*/
	bool irq_;
	bool nmi_;

	/*Interrupt lines are active low*/
	unsigned IRQn();
	unsigned NMIn();
	unsigned HandleInterrupts();

					/* ISA */
	typedef unsigned(Cpu::*Instruction)(uint8_t opcode);

	Instruction instructions_[ISA_SIZE]{};
	// Load-Store operations

	unsigned LDA(uint8_t opcode);	// Load Accumulator
	unsigned LDX(uint8_t opcode);	// Load X register
	unsigned LDY(uint8_t opcode);	// Load Y register
	unsigned STA(uint8_t opcode);	// Store Accumulator
	unsigned STX(uint8_t opcode);	// Store X Register
	unsigned STY(uint8_t opcode);	// Store Y Register

	// Register-Trasnfer operations

	unsigned TAX(uint8_t opcode);	// Transfer accumulator to X
	unsigned TAY(uint8_t opcode);	// Transfer accumulator to Y
	unsigned TXA(uint8_t opcode);	// Transfer X to accumulator
	unsigned TYA(uint8_t opcode);	// Transfer Y to accumulator

	// Stack Operations

	unsigned TSX(uint8_t opcode);	// Transfer stack pointer to X
	unsigned TXS(uint8_t opcode);	// Transfer X to stack pointer
	unsigned PHA(uint8_t opcode);	// Push accumulator on stack
	unsigned PHP(uint8_t opcode);   // Push processor status on stack   
	unsigned PLA(uint8_t opcode);   // Pull accumulator from stack 
	unsigned PLP(uint8_t opcode);   // Pull processor status from stack
	
	// Logical 

	unsigned AND(uint8_t opcode);   // Logical AND
	unsigned EOR(uint8_t opcode);   // Exclusive OR
	unsigned ORA(uint8_t opcode);	// Logical Inclusive OR	
	unsigned BIT(uint8_t opcode);   // Bit test 

	// Arithmetic

	unsigned ADC(uint8_t opcode);   // Add with Carry
	unsigned SBC(uint8_t opcode);   // Subtract with Carry    
	unsigned CMP(uint8_t opcode);   // Compare accumulator
	unsigned CPX(uint8_t opcode);   // Compare X register
	unsigned CPY(uint8_t opcode);   // Compare Y register 

	// Increments and Decrements

	unsigned INC(uint8_t opcode);   // Increment a memory location 
	unsigned INX(uint8_t opcode);   // Increment the X register
	unsigned INY(uint8_t opcode);   // Increment the Y register 
	unsigned DEX(uint8_t opcode);   // Decrement a memory location
	unsigned DEC(uint8_t opcode);   // Decrement the X register 
	unsigned DEY(uint8_t opcode);   // Decrement the Y register
	
	// Shifts

	unsigned ASL(uint8_t opcode);   // Arithmetic Shift Left
	unsigned LSR(uint8_t opcode);   // Logical Shift Right
	unsigned ROL(uint8_t opcode);   // Rotate Left
	unsigned ROR(uint8_t opcode);   // Rotate Right

	// Jumps and Calls

	unsigned JMP(uint8_t opcode);   // Jump to another location 
	unsigned JSR(uint8_t opcode);   // Jump to a subroutine 
	unsigned RTS(uint8_t opcode);   // Return from subroutine 
	
	// Branches
	
	unsigned BCC(uint8_t opcode);   // Branch if carry flag clear 
	unsigned BCS(uint8_t opcode);   // Branch if carry flag set 
	unsigned BEQ(uint8_t opcode);   // Branch if zero flag set
	unsigned BMI(uint8_t opcode);   // Branch if negative flag set 
	unsigned BNE(uint8_t opcode);   // Branch if zero flag clear 
	unsigned BPL(uint8_t opcode);   // Branch if negative flag clear
	unsigned BVC(uint8_t opcode);   // Branch if overflow flag clear 
	unsigned BVS(uint8_t opcode);   // Branch if overflow flag set

	// Status Flag Changes

	unsigned CLC(uint8_t opcode);   // Clear carry flag 
	unsigned CLD(uint8_t opcode);   // Clear decimal mode flag
	unsigned CLI(uint8_t opcode);   // Clear interrupt disable flag 
	unsigned CLV(uint8_t opcode);   // Clear overflow flag 
	unsigned SEC(uint8_t opcode);   // Set carry flag
	unsigned SED(uint8_t opcode);   // Set decimal mode flag 
	unsigned SEI(uint8_t opcode);   // Set interrupt disable flag

	//System Functions

	unsigned BRK(uint8_t opcode);   // Force an interrupt
	unsigned NOP(uint8_t opcode);   // No Operation
	unsigned RTI(uint8_t opcode);   // Return from Interrupt

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

