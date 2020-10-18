#include "cpu.hpp"

/* Alternative program counter for nestest*/
//#define ALT_PC 0xc000 

Cpu::Cpu() : clock_cycles_(0), irq_(true), nmi_(true)
{
	

	registers_ = new Registers;

	for (auto & instruction : instructions_) {
		instruction = nullptr;
	}

	instructions_[0x00] = &Cpu::BRK;
	instructions_[0x01] = &Cpu::ORA;
	instructions_[0x05] = &Cpu::ORA;
	instructions_[0x06] = &Cpu::ASL;
	instructions_[0x08] = &Cpu::PHP;
	instructions_[0x09] = &Cpu::ORA;
	instructions_[0x0a] = &Cpu::ASL;
	instructions_[0x0d] = &Cpu::ORA;
	instructions_[0x0e] = &Cpu::ASL;
	instructions_[0x10] = &Cpu::BPL;
	instructions_[0x11] = &Cpu::ORA;
	instructions_[0x15] = &Cpu::ORA;
	instructions_[0x16] = &Cpu::ASL;
	instructions_[0x18] = &Cpu::CLC;
	instructions_[0x19] = &Cpu::ORA;
	instructions_[0x1d] = &Cpu::ORA;
	instructions_[0x1e] = &Cpu::ASL;
	instructions_[0x20] = &Cpu::JSR;
	instructions_[0x21] = &Cpu::AND;
	instructions_[0x24] = &Cpu::BIT;
	instructions_[0x25] = &Cpu::AND;
	instructions_[0x26] = &Cpu::ROL;
	instructions_[0x28] = &Cpu::PLP;
	instructions_[0x29] = &Cpu::AND;
	instructions_[0x2a] = &Cpu::ROL;
	instructions_[0x2c] = &Cpu::BIT;
	instructions_[0x2d] = &Cpu::AND;
	instructions_[0x2e] = &Cpu::ROL;
	instructions_[0x30] = &Cpu::BMI;
	instructions_[0x31] = &Cpu::AND;
	instructions_[0x35] = &Cpu::AND;
	instructions_[0x36] = &Cpu::ROL;
	instructions_[0x38] = &Cpu::SEC;
	instructions_[0x39] = &Cpu::AND;
	instructions_[0x3d] = &Cpu::AND;
	instructions_[0x3e] = &Cpu::ROL;
	instructions_[0x40] = &Cpu::RTI;
	instructions_[0x41] = &Cpu::EOR;
	instructions_[0x45] = &Cpu::EOR;
	instructions_[0x46] = &Cpu::LSR;
	instructions_[0x48] = &Cpu::PHA;
	instructions_[0x49] = &Cpu::EOR;
	instructions_[0x4a] = &Cpu::LSR;
	instructions_[0x4c] = &Cpu::JMP;
	instructions_[0x4d] = &Cpu::EOR;
	instructions_[0x4e] = &Cpu::LSR;
	instructions_[0x50] = &Cpu::BVC;
	instructions_[0x51] = &Cpu::EOR;
	instructions_[0x55] = &Cpu::EOR;
	instructions_[0x56] = &Cpu::LSR;
	instructions_[0x58] = &Cpu::CLI;
	instructions_[0x59] = &Cpu::EOR;
	instructions_[0x5d] = &Cpu::EOR;
	instructions_[0x5e] = &Cpu::LSR;
	instructions_[0x60] = &Cpu::RTS;
	instructions_[0x61] = &Cpu::ADC;
	instructions_[0x65] = &Cpu::ADC;
	instructions_[0x66] = &Cpu::ROR;
	instructions_[0x68] = &Cpu::PLA;
	instructions_[0x69] = &Cpu::ADC;
	instructions_[0x6a] = &Cpu::ROR;
	instructions_[0x6c] = &Cpu::JMP;
	instructions_[0x6d] = &Cpu::ADC;
	instructions_[0x6e] = &Cpu::ROR;
	instructions_[0x70] = &Cpu::BVS;
	instructions_[0x71] = &Cpu::ADC;
	instructions_[0x75] = &Cpu::ADC;
	instructions_[0x76] = &Cpu::ROR;
	instructions_[0x78] = &Cpu::SEI;
	instructions_[0x79] = &Cpu::ADC;
	instructions_[0x7d] = &Cpu::ADC;
	instructions_[0x7e] = &Cpu::ROR;
	instructions_[0x81] = &Cpu::STA;
	instructions_[0x84] = &Cpu::STY;
	instructions_[0x85] = &Cpu::STA;
	instructions_[0x86] = &Cpu::STX;
	instructions_[0x88] = &Cpu::DEY;
	instructions_[0x8a] = &Cpu::TXA;
	instructions_[0x8c] = &Cpu::STY;
	instructions_[0x8d] = &Cpu::STA;
	instructions_[0x8e] = &Cpu::STX;
	instructions_[0x90] = &Cpu::BCC;
	instructions_[0x91] = &Cpu::STA;
	instructions_[0x94] = &Cpu::STY;
	instructions_[0x95] = &Cpu::STA;
	instructions_[0x96] = &Cpu::STX;
	instructions_[0x98] = &Cpu::TYA;
	instructions_[0x99] = &Cpu::STA;
	instructions_[0x9a] = &Cpu::TXS;
	instructions_[0x9d] = &Cpu::STA;
	instructions_[0xa0] = &Cpu::LDY;
	instructions_[0xa1] = &Cpu::LDA;
	instructions_[0xa2] = &Cpu::LDX;
	instructions_[0xa4] = &Cpu::LDY;
	instructions_[0xa5] = &Cpu::LDA;
	instructions_[0xa6] = &Cpu::LDX;
	instructions_[0xa8] = &Cpu::TAY;
	instructions_[0xa9] = &Cpu::LDA;
	instructions_[0xaa] = &Cpu::TAX;
	instructions_[0xac] = &Cpu::LDY;
	instructions_[0xad] = &Cpu::LDA;
	instructions_[0xae] = &Cpu::LDX;
	instructions_[0xb0] = &Cpu::BCS;
	instructions_[0xb1] = &Cpu::LDA;
	instructions_[0xb4] = &Cpu::LDY;
	instructions_[0xb5] = &Cpu::LDA;
	instructions_[0xb6] = &Cpu::LDX;
	instructions_[0xb8] = &Cpu::CLV;
	instructions_[0xb9] = &Cpu::LDA;
	instructions_[0xba] = &Cpu::TSX;
	instructions_[0xbc] = &Cpu::LDY;
	instructions_[0xbd] = &Cpu::LDA;
	instructions_[0xbe] = &Cpu::LDX;
	instructions_[0xc0] = &Cpu::CPY;
	instructions_[0xc1] = &Cpu::CMP;
	instructions_[0xc4] = &Cpu::CPY;
	instructions_[0xc5] = &Cpu::CMP;
	instructions_[0xc6] = &Cpu::DEC;
	instructions_[0xc8] = &Cpu::INY;
	instructions_[0xc9] = &Cpu::CMP;
	instructions_[0xca] = &Cpu::DEX;
	instructions_[0xcc] = &Cpu::CPY;
	instructions_[0xcd] = &Cpu::CMP;
	instructions_[0xce] = &Cpu::DEC;
	instructions_[0xd0] = &Cpu::BNE;
	instructions_[0xd1] = &Cpu::CMP;
	instructions_[0xd5] = &Cpu::CMP;
	instructions_[0xd6] = &Cpu::DEC;
	instructions_[0xd8] = &Cpu::CLD;
	instructions_[0xd9] = &Cpu::CMP;
	instructions_[0xdd] = &Cpu::CMP;
	instructions_[0xde] = &Cpu::DEC;
	instructions_[0xe0] = &Cpu::CPX;
	instructions_[0xe1] = &Cpu::SBC;
	instructions_[0xe4] = &Cpu::CPX;
	instructions_[0xe5] = &Cpu::SBC;
	instructions_[0xe6] = &Cpu::INC;
	instructions_[0xe8] = &Cpu::INX;
	instructions_[0xe9] = &Cpu::SBC;
	instructions_[0xea] = &Cpu::NOP;
	instructions_[0xec] = &Cpu::CPX;
	instructions_[0xed] = &Cpu::SBC;
	instructions_[0xee] = &Cpu::INC;
	instructions_[0xf0] = &Cpu::BEQ;
	instructions_[0xf1] = &Cpu::SBC;
	instructions_[0xf5] = &Cpu::SBC;
	instructions_[0xf6] = &Cpu::INC;
	instructions_[0xf8] = &Cpu::SED;
	instructions_[0xf9] = &Cpu::SBC;
	instructions_[0xfd] = &Cpu::SBC;
	instructions_[0xfe] = &Cpu::INC;
}

void Cpu::Init(Mmu* mmu, Ppu* ppu) {
	this->mmu_ = mmu;
	this->ppu_ = ppu;
}


Cpu::~Cpu() {
	Logger::Log("Cpu destructor called");
	delete registers_;
}

uint64_t Cpu::Step()
{

	unsigned op_cycles = 0;

	uint16_t pc = registers_->pc_;
	uint8_t opcode = mmu_->ReadByte(pc);

	DebugLogger::LogOp(registers_, mmu_, clock_cycles_);

	if (instructions_[opcode] != nullptr) {
		op_cycles = (this->*instructions_[opcode])(opcode);
	}
	else {
		Logger::LogError("Unknown OPcode: 0x%02X at PC = 0x%04X", (unsigned)opcode, (unsigned)registers_->pc_);
		exit(EXIT_FAILURE);
	}

	/* If an interrupt is present, calling the ISR costs 7 cycles*/
	op_cycles += HandleInterrupts();

	/* Ppu sync*/
	ppu_->Step(op_cycles);

	/* Global clock cycles counter*/
	clock_cycles_ += op_cycles;
	
	return op_cycles;
}

void Cpu::Reset()
{
	// Reset Interrupt procedure. Called on Power Up or reset button pressed 

	/*RESET is how code even begin executing on the 6502 when powered on or reset. 
	The CPU reads the 2 bytes at $FFFC-FFFD (RESET vector), in the usual little endian format, 
	sets the PC to the values in those bytes, then begins executing code from there. 
	If this makes it easier: you can think of the RESET vector as the 
	CPU essentially doing jmp ($fffc) when it powers on or is reset*/
	//registers_->Reset();

	registers_->a_ = 0x00;
	registers_->x_ = 0x00;
	registers_->y_ = 0x00;
	registers_->p_ = 0x24;
	/*stack pointer is decremented by 3 without writing anything on the stack*/
	registers_->s_ = 0xfd;
	/* Only for debug puproses. For example running nestest in "automation" mode requires the
	PC to start from 0xc000. So can be useful to easily switch from the default way of
	defining the PC at reset and the debug mode that defines an alternative PC*/
	#ifdef ALT_PC
		registers_->pc_ = ALT_PC;
	#else
	registers_->pc_= mmu_->ReadWord(RESET_BASE);
	#endif

	clock_cycles_ += 7;
}

unsigned Cpu::IRQn() {
	/*Interrupt request
	An IRQ does basically the same thing as a BRK, 
	but it clears the B flag in the pushed status byte. 
	The CPU goes through the same sequence of cycles as in the BRK case
	*/

	// push pc
	mmu_->WriteByte((uint16_t)registers_->s_ + STACK_BASE, (uint8_t)(registers_->pc_ >> 8));
	mmu_->WriteByte((uint16_t)registers_->s_ + STACK_BASE - 1, (uint8_t)(registers_->pc_ & 0x00ff));
	registers_->s_ -= 2;

	// push p with B at 0
	mmu_->WriteByte((uint16_t)registers_->s_ + STACK_BASE, registers_->p_ & 0b11101111);
	registers_->s_ -= 1;

	// set Ird to avoid another interrupt
	registers_->SetIrd();

	// load IRQ interrupt vector
	registers_->pc_ = mmu_->ReadWord(IRQ_BASE);

	return 7;
}

unsigned Cpu::NMIn() {
	/*Non-maskable interrupt request*/

	// push pc
	mmu_->WriteByte((uint16_t)registers_->s_ + STACK_BASE, (uint8_t)(registers_->pc_ >> 8));
	mmu_->WriteByte((uint16_t)registers_->s_ + STACK_BASE - 1, (uint8_t)(registers_->pc_ & 0x00ff));
	registers_->s_ -= 2;

	// push p with B at 0
	mmu_->WriteByte((uint16_t)registers_->s_ + STACK_BASE, registers_->p_ & 0b11101111);
	registers_->s_ -= 1;

	// set Ird to avoid another interrupt
	registers_->SetIrd();

	// load IRQ interrupt vector
	registers_->pc_ = mmu_->ReadWord(NMI_BASE);

	return 7;
}

unsigned Cpu::HandleInterrupts() {

	if (!nmi_) {
		return NMIn();
	}

	else if (!registers_->CheckIrd() && !irq_) {
		/* Interrupts are not masked*/
		return IRQn();
	}

	return 0;
}

unsigned Cpu::LDA(uint8_t opcode)
{
	uint8_t cycles = 0;
	uint8_t operand = 0x00;
	bool is_crossed = false;
	int8_t pc_inc = 0;
	uint16_t addr = 0x0000;

	switch (opcode) {
	case 0xa9:
		// Immediate
		Immediate(addr, pc_inc);
		cycles = 2;
		break;
	case 0xa5:
		// Zero page
		ZeroPage(addr, pc_inc);
		cycles = 3;
		break;
	case 0xb5:
		// Zero page,X
		ZeroPageX(addr, pc_inc);
		cycles = 4;
		break;
	case 0xad:
		// Absolute
		Absolute(addr, pc_inc);
		cycles = 4;
		break;
	case 0xbd:
		// Absolute,X
		is_crossed = AbsoluteX(addr, pc_inc);
		cycles = 4;
		if (is_crossed) {
			cycles++;
		}
		break;
	case 0xb9:
		// Absolute,Y
		is_crossed = AbsoluteY(addr, pc_inc);
		cycles = 4;
		if (is_crossed) {
			cycles++;
		}
		break;
	case 0xa1:
		// (Indirect,X).
		IndexedIndirect(addr, pc_inc);
		cycles = 6;
		break;
	case 0xb1:
		// (Indirect),Y
		is_crossed = IndirectIndexed(addr, pc_inc);
		cycles = 5;
		if (is_crossed) {
			cycles++;
		}
		break;
	default:
		Logger::LogError("Unknown OPcode: 0x%02X at PC = 0x%04X", (unsigned)opcode, (unsigned)registers_->pc_);
		exit(EXIT_FAILURE);
	}


	operand = mmu_->ReadByte(addr);

	// Flag manipulation
	
	CheckNegative(operand) ? registers_->SetNeg() : registers_->ClearNeg();
	(operand == 0) ? registers_->SetZer() : registers_->ClearZer();

	registers_->a_ = operand;
	registers_->pc_ += pc_inc;

	return cycles;
}

unsigned Cpu::LDX(uint8_t opcode)
{
	uint8_t cycles = 0;
	uint8_t operand = 0x00;
	bool is_crossed = false;
	int8_t pc_inc = 0;
	uint16_t addr = 0x0000;

	switch (opcode) {
	case 0xa2:
		// Immediate
		Immediate(addr, pc_inc);
		cycles = 2;
		break;
	case 0xa6:
		// Zero page
		ZeroPage(addr, pc_inc);
		cycles = 3;
		break;
	case 0xb6:
		// Zero page,X
		ZeroPageY(addr, pc_inc);
		cycles = 4;
		break;
	case 0xae:
		// Absolute 
		Absolute(addr, pc_inc);
		cycles = 4;
		break;
	case 0xbe:
		// Absolute,X
		is_crossed = AbsoluteY(addr, pc_inc);
		cycles = 4;
		if (is_crossed) {
			cycles++;
		}
		break;
	default:
		Logger::LogError("Unknown OPcode: 0x%02X at PC = 0x%04X", (unsigned)opcode, (unsigned)registers_->pc_);
		exit(EXIT_FAILURE);
	}

	operand = mmu_->ReadByte(addr);

	CheckNegative(operand) ? registers_->SetNeg() : registers_->ClearNeg();
	(operand == 0) ? registers_->SetZer() : registers_->ClearZer();

	registers_->x_ = operand;
	registers_->pc_ += pc_inc;

	return cycles;
}

unsigned Cpu::LDY(uint8_t opcode)
{
	uint8_t cycles = 0;
	uint8_t operand = 0x00;
	bool is_crossed = false;
	int8_t pc_inc = 0;
	uint16_t addr = 0x0000;

	switch (opcode)
	{
	case 0xa0:
		// Immediate
		Immediate(addr, pc_inc);
		cycles = 2;
		break;
	case 0xa4:
		// Zero Page 
		ZeroPage(addr, pc_inc);
		cycles = 3;
		break;
	case 0xb4:
		// Zero Page, X 
		ZeroPageX(addr, pc_inc);
		cycles = 4;
		break;
	case 0xac:
		// Absolute 
		Absolute(addr, pc_inc);
		cycles = 4;
		break;
	case 0xbc:
		// Absolute, X 
		is_crossed = AbsoluteX(addr, pc_inc);
		cycles = 4;
		if (is_crossed) {
			cycles++;
		}
		break;
	default:
		Logger::LogError("Unknown OPcode: 0x%02X at PC = 0x%04X", (unsigned)opcode, (unsigned)registers_->pc_);
		exit(EXIT_FAILURE);
	}

	operand = mmu_->ReadByte(addr);

	CheckNegative(operand) ? registers_->SetNeg() : registers_->ClearNeg();
	(operand == 0) ? registers_->SetZer() : registers_->ClearZer();

	registers_->y_ = operand;
	registers_->pc_ += pc_inc;

	return cycles;
}

unsigned Cpu::STA(uint8_t opcode)
{
	uint8_t cycles = 0;
	int8_t pc_inc = 0;
	uint16_t addr = 0x0000;

	switch (opcode) 
	{
	case 0x85:
		// Zero Page
		ZeroPage(addr, pc_inc);
		cycles = 3;
		break;
	case 0x95:
		// Zero Page,X
		ZeroPageX(addr, pc_inc);
		cycles = 4;
		break;
	case 0x8d:
		// Absolute
		Absolute(addr, pc_inc);
		cycles = 4;
		break;
	case 0x9d:
		// Absolute,X
		AbsoluteX(addr, pc_inc);
		cycles = 5;
		break;
	case 0x99:
		// Absolute,Y
		AbsoluteY(addr, pc_inc);
		cycles = 5;
		break;
	case 0x81:
		// (Indirect,X)
		IndexedIndirect(addr, pc_inc);
		cycles = 6;
		break;
	case 0x91:
		// (Indirect),Y
		IndirectIndexed(addr, pc_inc);
		cycles = 6;
		break;
	default:
		Logger::LogError("Unknown OPcode: 0x%02X at PC = 0x%04X", (unsigned)opcode, (unsigned)registers_->pc_);
		exit(EXIT_FAILURE);
	}

	mmu_->WriteByte(addr, registers_->a_);
	registers_->pc_ += pc_inc;

	return cycles;
}

unsigned Cpu::STX(uint8_t opcode)
{
	uint8_t cycles = 0;
	int8_t pc_inc = 0;
	uint16_t addr = 0x0000;

	switch (opcode)
	{
	case 0x86:
		// Zero Page
		ZeroPage(addr, pc_inc);
		cycles = 3;
		break;
	case 0x96:
		// Zero Page,Y
		ZeroPageY(addr, pc_inc);
		cycles = 4;
		break;
	case 0x8e:
		// Absolute
		Absolute(addr, pc_inc);
		cycles = 4;
		break;
	default:
		Logger::LogError("Unknown OPcode: 0x%02X at PC = 0x%04X", (unsigned)opcode, (unsigned)registers_->pc_);
		exit(EXIT_FAILURE);
	}

	mmu_->WriteByte(addr, registers_->x_);
	registers_->pc_ += pc_inc;

	return cycles;
}

unsigned Cpu::STY(uint8_t opcode)
{
	uint8_t cycles = 0;
	int8_t pc_inc = 0;
	uint16_t addr = 0x0000;

	switch (opcode)
	{
	case 0x84:
		// Zero Page
		ZeroPage(addr, pc_inc);
		cycles = 3;
		break;
	case 0x94:
		// Zero Page,X
		ZeroPageX(addr, pc_inc);
		cycles = 4;
		break;
	case 0x8c:
		// Absolute
		Absolute(addr, pc_inc);
		cycles = 4;
		break;
	default:
		Logger::LogError("Unknown OPcode: 0x%02X at PC = 0x%04X", (unsigned)opcode, (unsigned)registers_->pc_);
		exit(EXIT_FAILURE);
	}

	mmu_->WriteByte(addr, registers_->y_);
	registers_->pc_ += pc_inc;

	return cycles;
}

unsigned Cpu::TAX(uint8_t opcode)
{
	(void)opcode; //suppress warnings

	uint8_t operand = registers_->a_;

	CheckNegative(operand) ? registers_->SetNeg() : registers_->ClearNeg();
	(operand == 0) ? registers_->SetZer() : registers_->ClearZer();

	registers_->x_ = operand;
	registers_->pc_ += 1;

	return 2;
}

unsigned Cpu::TAY(uint8_t opcode)
{
	(void)opcode; //suppress warnings

	uint8_t operand = registers_->a_;

	CheckNegative(operand) ? registers_->SetNeg() : registers_->ClearNeg();
	(operand == 0) ? registers_->SetZer() : registers_->ClearZer();

	registers_->y_ = operand;
	registers_->pc_ += 1;

	return 2;
}

unsigned Cpu::TXA(uint8_t opcode)
{
	(void)opcode; //suppress warnings

	uint8_t operand = registers_->x_;

	CheckNegative(operand) ? registers_->SetNeg() : registers_->ClearNeg();
	(operand == 0) ? registers_->SetZer() : registers_->ClearZer();

	registers_->a_ = operand;
	registers_->pc_ += 1;

	return 2;
}

unsigned Cpu::TYA(uint8_t opcode)
{
	(void)opcode; //suppress warnings

	uint8_t operand = registers_->y_;

	CheckNegative(operand) ? registers_->SetNeg() : registers_->ClearNeg();
	(operand == 0) ? registers_->SetZer() : registers_->ClearZer();

	registers_->a_ = operand;
	registers_->pc_ += 1;

	return 2;
}

unsigned Cpu::TSX(uint8_t opcode)
{
	(void)opcode; //suppress warnings

	uint8_t operand = registers_->s_;

	CheckNegative(operand) ? registers_->SetNeg() : registers_->ClearNeg();
	(operand == 0) ? registers_->SetZer() : registers_->ClearZer();

	registers_->x_ = operand;
	registers_->pc_ += 1;

	return 2;
}

unsigned Cpu::TXS(uint8_t opcode)
{
	(void)opcode; //suppress warnings

	uint8_t operand = registers_->x_;

	// Don't know if flags are set
	//CheckNegative(operand) ? registers_->SetNeg() : registers_->ClearNeg();
	//(operand == 0) ? registers_->SetZer() : registers_->ClearZer();

	registers_->s_ = operand;
	registers_->pc_ += 1;

	return 2;
}

unsigned Cpu::PHA(uint8_t opcode)
{
	(void)opcode; //suppress warnings

	uint8_t operand = registers_->a_;
	uint16_t addr = (uint16_t)registers_->s_ + STACK_BASE;
	
	mmu_->WriteByte(addr, operand);
	registers_->s_ -= 1;
	registers_->pc_ += 1;

	return 3;
}

unsigned Cpu::PHP(uint8_t opcode)
{
	(void)opcode; //suppress warnings

	uint8_t operand = registers_->p_ | 0b00110000;
	uint16_t addr = (uint16_t)registers_->s_ + STACK_BASE;

	mmu_->WriteByte(addr, operand);
	registers_->s_ -= 1;
	registers_->pc_ += 1;

	return 3;
}

unsigned Cpu::PLA(uint8_t opcode)
{
	(void)opcode; //suppress warnings

	registers_->s_ += 1;

	uint16_t addr = (uint16_t)registers_->s_ + STACK_BASE;
	uint8_t operand = mmu_->ReadByte(addr);

	CheckNegative(operand) ? registers_->SetNeg() : registers_->ClearNeg();
	(operand == 0) ? registers_->SetZer() : registers_->ClearZer();

	registers_->a_ = operand;
	registers_->pc_ += 1;

	return 4;
}

unsigned Cpu::PLP(uint8_t opcode)
{
	(void)opcode; //suppress warnings

	// Two instructions (PLP and RTI) pull a byte from the stack and set all the flags. They ignore bits 5 and 4. 

	registers_->s_ += 1;

	uint16_t addr = (uint16_t)registers_->s_ + STACK_BASE;
	uint8_t operand = mmu_->ReadByte(addr);

	CheckBit(operand, NEG) ? registers_->SetNeg() : registers_->ClearNeg();
	CheckBit(operand, OVF) ? registers_->SetOvf() : registers_->ClearOvf();
	CheckBit(operand, DEM) ? registers_->SetDec() : registers_->ClearDec();
	CheckBit(operand, IRQ) ? registers_->SetIrd() : registers_->ClearIrd();
	CheckBit(operand, ZER) ? registers_->SetZer() : registers_->ClearZer();
	CheckBit(operand, CAR) ? registers_->SetCar() : registers_->ClearCar();

	registers_->pc_ += 1;

	return 4;
}

unsigned Cpu::AND(uint8_t opcode)
{
	uint8_t cycles = 0;
	uint8_t operand = 0x00;
	bool is_crossed = false;
	int8_t pc_inc = 0;
	uint16_t addr = 0x0000;

	switch (opcode) {
	case 0x29:
		// Immediate
		Immediate(addr, pc_inc);
		cycles = 2;
		break;
	case 0x25:
		// Zero page
		ZeroPage(addr, pc_inc);
		cycles = 3;
		break;
	case 0x35:
		// Zero page,X
		ZeroPageX(addr, pc_inc);
		cycles = 4;
		break;
	case 0x2d:
		// Absolute
		Absolute(addr, pc_inc);
		cycles = 4;
		break;
	case 0x3d:
		// Absolute,X
		is_crossed = AbsoluteX(addr, pc_inc);
		cycles = 4;
		if (is_crossed) {
			cycles++;
		}
		break;
	case 0x39:
		// Absolute,Y
		is_crossed = AbsoluteY(addr, pc_inc);
		cycles = 4;
		if (is_crossed) {
			cycles++;
		}
		break;
	case 0x21:
		// (Indirect,X).
		IndexedIndirect(addr, pc_inc);
		cycles = 6;
		break;
	case 0x31:
		// (Indirect),Y
		is_crossed = IndirectIndexed(addr, pc_inc);
		cycles = 5;
		if (is_crossed) {
			cycles++;
		}
		break;
	default:
		Logger::LogError("Unknown OPcode: 0x%02X at PC = 0x%04X", (unsigned)opcode, (unsigned)registers_->pc_);
		exit(EXIT_FAILURE);
	}

	operand = mmu_->ReadByte(addr);

	// Actual operation
	uint8_t res = registers_->a_ & operand;

	CheckNegative(res) ? registers_->SetNeg() : registers_->ClearNeg();
	(res == 0) ? registers_->SetZer() : registers_->ClearZer();

	registers_->a_ = res;
	registers_->pc_ += pc_inc;

	return cycles;
}

unsigned Cpu::EOR(uint8_t opcode)
{
	uint8_t cycles = 0;
	uint8_t operand = 0x00;
	bool is_crossed = false;
	int8_t pc_inc = 0;
	uint16_t addr = 0x0000;

	switch (opcode) {
	case 0x49:
		// Immediate
		Immediate(addr, pc_inc);
		cycles = 2;
		break;
	case 0x45:
		// Zero page
		ZeroPage(addr, pc_inc);
		cycles = 3;
		break;
	case 0x55:
		// Zero page,X
		ZeroPageX(addr, pc_inc);
		cycles = 4;
		break;
	case 0x4d:
		// Absolute
		Absolute(addr, pc_inc);
		cycles = 4;
		break;
	case 0x5d:
		// Absolute,X
		is_crossed = AbsoluteX(addr, pc_inc);
		cycles = 4;
		if (is_crossed) {
			cycles++;
		}
		break;
	case 0x59:
		// Absolute,Y
		is_crossed = AbsoluteY(addr, pc_inc);
		cycles = 4;
		if (is_crossed) {
			cycles++;
		}
		break;
	case 0x41:
		// (Indirect,X).
		IndexedIndirect(addr, pc_inc);
		cycles = 6;
		break;
	case 0x51:
		// (Indirect),Y
		is_crossed = IndirectIndexed(addr, pc_inc);
		cycles = 5;
		if (is_crossed) {
			cycles++;
		}
		break;
	default:
		Logger::LogError("Unknown OPcode: 0x%02X at PC = 0x%04X", (unsigned)opcode, (unsigned)registers_->pc_);
		exit(EXIT_FAILURE);
	}

	operand = mmu_->ReadByte(addr);

	// Actual operation
	uint8_t res = registers_->a_ ^ operand;

	CheckNegative(res) ? registers_->SetNeg() : registers_->ClearNeg();
	(res == 0) ? registers_->SetZer() : registers_->ClearZer();

	registers_->a_ = res;
	registers_->pc_ += pc_inc;

	return cycles;
}

unsigned Cpu::ORA(uint8_t opcode)
{
	uint8_t cycles = 0;
	uint8_t operand = 0x00;
	bool is_crossed = false;
	int8_t pc_inc = 0;
	uint16_t addr = 0x0000;

	switch (opcode) {
	case 0x09:
		// Immediate
		Immediate(addr, pc_inc);
		cycles = 2;
		break;
	case 0x05:
		// Zero page
		ZeroPage(addr, pc_inc);
		cycles = 3;
		break;
	case 0x15:
		// Zero page,X
		ZeroPageX(addr, pc_inc);
		cycles = 4;
		break;
	case 0x0d:
		// Absolute
		Absolute(addr, pc_inc);
		cycles = 4;
		break;
	case 0x1d:
		// Absolute,X
		is_crossed = AbsoluteX(addr, pc_inc);
		cycles = 4;
		if (is_crossed) {
			cycles++;
		}
		break;
	case 0x19:
		// Absolute,Y
		is_crossed = AbsoluteY(addr, pc_inc);
		cycles = 4;
		if (is_crossed) {
			cycles++;
		}
		break;
	case 0x01:
		// (Indirect,X).
		IndexedIndirect(addr, pc_inc);
		cycles = 6;
		break;
	case 0x11:
		// (Indirect),Y
		is_crossed = IndirectIndexed(addr, pc_inc);
		cycles = 5;
		if (is_crossed) {
			cycles++;
		}
		break;
	default:
		Logger::LogError("Unknown OPcode: 0x%02X at PC = 0x%04X", (unsigned)opcode, (unsigned)registers_->pc_);
		exit(EXIT_FAILURE);
	}

	operand = mmu_->ReadByte(addr);

	// Actual operation
	uint8_t res = registers_->a_ | operand;

	CheckNegative(res) ? registers_->SetNeg() : registers_->ClearNeg();
	(res == 0) ? registers_->SetZer() : registers_->ClearZer();

	registers_->a_ = res;
	registers_->pc_ += pc_inc;

	return cycles;
}

unsigned Cpu::BIT(uint8_t opcode)
{
	/*The mask pattern in A is ANDed with the value in memory to set or clear the zero flag, 
	but the result is not kept. Bits 7 and 6 of the value from memory are copied into the N and V flags.*/

	uint8_t cycles = 0;
	uint8_t operand = 0x00;
	int8_t pc_inc = 0;
	uint16_t addr = 0x0000;

	switch (opcode)
	{
	case 0x24:
		ZeroPage(addr, pc_inc);
		cycles = 3;
		break;
	case 0x2c:
		Absolute(addr, pc_inc);
		cycles = 4;
		break;
	default:
		Logger::LogError("Unknown OPcode: 0x%02X at PC = 0x%04X", (unsigned)opcode, (unsigned)registers_->pc_);
		exit(EXIT_FAILURE);
	}

	operand = mmu_->ReadByte(addr);

	// Actual operation
	uint8_t res = registers_->a_ & operand;

	(res == 0) ? registers_->SetZer() : registers_->ClearZer();
	CheckNegative(operand) ? registers_->SetNeg() : registers_->ClearNeg();
	CheckBit(operand, 6) ? registers_->SetOvf() : registers_->ClearOvf();

	registers_->pc_ += pc_inc;

	return cycles;
}

unsigned Cpu::ADC(uint8_t opcode)
{
	uint8_t cycles = 0;
	uint8_t operand = 0x00;
	bool is_crossed = false;
	int8_t pc_inc = 0;
	uint16_t addr = 0x0000;

	switch (opcode) {
	case 0x69:
		// Immediate
		Immediate(addr, pc_inc);
		cycles = 2;
		break;
	case 0x65:
		// Zero page
		ZeroPage(addr, pc_inc);
		cycles = 3;
		break;
	case 0x75:
		// Zero page,X
		ZeroPageX(addr, pc_inc);
		cycles = 4;
		break;
	case 0x6d:
		// Absolute
		Absolute(addr, pc_inc);
		cycles = 4;
		break;
	case 0x7d:
		// Absolute,X
		is_crossed = AbsoluteX(addr, pc_inc);
		cycles = 4;
		if (is_crossed) {
			cycles++;
		}
		break;
	case 0x79:
		// Absolute,Y
		is_crossed = AbsoluteY(addr, pc_inc);
		cycles = 4;
		if (is_crossed) {
			cycles++;
		}
		break;
	case 0x61:
		// (Indirect,X).
		IndexedIndirect(addr, pc_inc);
		cycles = 6;
		break;
	case 0x71:
		// (Indirect),Y
		is_crossed = IndirectIndexed(addr, pc_inc);
		cycles = 5;
		if (is_crossed) {
			cycles++;
		}
		break;
	default:
		Logger::LogError("Unknown OPcode: 0x%02X at PC = 0x%04X", (unsigned)opcode, (unsigned)registers_->pc_);
		exit(EXIT_FAILURE);
	}

	operand = mmu_->ReadByte(addr);

	// Actual operation
	uint8_t carry = registers_->CheckCar();
	uint8_t res = registers_->a_ + operand + carry;

	CheckNegative(res) ? registers_->SetNeg() : registers_->ClearNeg();
	(res == 0) ? registers_->SetZer() : registers_->ClearZer();
	CheckOverflow(registers_->a_, operand, res) ? registers_->SetOvf() : registers_->ClearOvf();
	CheckCarry(registers_->a_, operand + carry) ? registers_->SetCar() : registers_->ClearCar();

	registers_->a_ = res;
	registers_->pc_ += pc_inc;

	return cycles;
}

unsigned Cpu::SBC(uint8_t opcode)
{
	uint8_t cycles = 0;
	uint8_t operand = 0x00;
	bool is_crossed = false;
	int8_t pc_inc = 0;
	uint16_t addr = 0x0000;

	switch (opcode) {
	case 0xe9:
		// Immediate
		Immediate(addr, pc_inc);
		cycles = 2;
		break;
	case 0xe5:
		// Zero page
		ZeroPage(addr, pc_inc);
		cycles = 3;
		break;
	case 0xf5:
		// Zero page,X
		ZeroPageX(addr, pc_inc);
		cycles = 4;
		break;
	case 0xed:
		// Absolute
		Absolute(addr, pc_inc);
		cycles = 4;
		break;
	case 0xfd:
		// Absolute,X
		is_crossed = AbsoluteX(addr, pc_inc);
		cycles = 4;
		if (is_crossed) {
			cycles++;
		}
		break;
	case 0xf9:
		// Absolute,Y
		is_crossed = AbsoluteY(addr, pc_inc);
		cycles = 4;
		if (is_crossed) {
			cycles++;
		}
		break;
	case 0xe1:
		// (Indirect,X).
		IndexedIndirect(addr, pc_inc);
		cycles = 6;
		break;
	case 0xf1:
		// (Indirect),Y
		is_crossed = IndirectIndexed(addr, pc_inc);
		cycles = 5;
		if (is_crossed) {
			cycles++;
		}
		break;
	default:
		Logger::LogError("Unknown OPcode: 0x%02X at PC = 0x%04X", (unsigned)opcode, (unsigned)registers_->pc_);
		exit(EXIT_FAILURE);
	}

	operand = mmu_->ReadByte(addr);

	// Actual operation
	uint8_t not_carry = !(registers_->CheckCar());
	uint8_t res = registers_->a_ - operand - not_carry;

	CheckNegative(res) ? registers_->SetNeg() : registers_->ClearNeg();
	(res == 0) ? registers_->SetZer() : registers_->ClearZer();

	/*The SBC instruction is similar to the ADC in that in involves the same three entities - 
	the accumulator, the carry flag, and a memory location - but dissimilar 
	in that it is not the value of the carry flag that is subtracted but its complement. 
	After the subtraction the carry flag is set if no borrow was required 
	(the value in the accumulator was at least as large as the value in memory) 
	or clear if a borrow was required (the value in the accumulator was smaller than the value in memory). 
	As with addition, the state of the carry flag after a first subtraction can be used 
	to "propogate" a ninth bit across multiple eight-bit subtractions.
	If the carry is cleared by the operation, it indicates a borrow occurred. */

	uint8_t operand_ca2 = ((operand + not_carry) ^ 0xff) + 1; /*add the !carry and then 2's complement the result. In this way the computed result is correct*/

	CheckOverflow(registers_->a_, operand_ca2, res) ? registers_->SetOvf() : registers_->ClearOvf(); 
	CheckBorrow(registers_->a_, operand) ? registers_->ClearCar() : registers_->SetCar();

	registers_->a_ = res;
	registers_->pc_ += pc_inc;


	return cycles;
}

unsigned Cpu::CMP(uint8_t opcode)
{
	uint8_t cycles = 0;
	uint8_t operand = 0x00;
	bool is_crossed = false;
	int8_t pc_inc = 0;
	uint16_t addr = 0x0000;

	switch (opcode) {
	case 0xc9:
		// Immediate
		Immediate(addr, pc_inc);
		cycles = 2;
		break;
	case 0xc5:
		// Zero page
		ZeroPage(addr, pc_inc);
		cycles = 3;
		break;
	case 0xd5:
		// Zero page,X
		ZeroPageX(addr, pc_inc);
		cycles = 4;
		break;
	case 0xcd:
		// Absolute
		Absolute(addr, pc_inc);
		cycles = 4;
		break;
	case 0xdd:
		// Absolute,X
		is_crossed = AbsoluteX(addr, pc_inc);
		cycles = 4;
		if (is_crossed) {
			cycles++;
		}
		break;
	case 0xd9:
		// Absolute,Y
		is_crossed = AbsoluteY(addr, pc_inc);
		cycles = 4;
		if (is_crossed) {
			cycles++;
		}
		break;
	case 0xc1:
		// (Indirect,X).
		IndexedIndirect(addr, pc_inc);
		cycles = 6;
		break;
	case 0xd1:
		// (Indirect),Y
		is_crossed = IndirectIndexed(addr, pc_inc);
		cycles = 5;
		if (is_crossed) {
			cycles++;
		}
		break;
	default:
		Logger::LogError("Unknown OPcode: 0x%02X at PC = 0x%04X", (unsigned)opcode, (unsigned)registers_->pc_);
		exit(EXIT_FAILURE);
	}

	operand = mmu_->ReadByte(addr);

	// Actual operation
	uint8_t res = registers_->a_ - operand;

	//CheckNegative(res) ? registers_->ClearCar() : registers_->SetCar();
	(registers_->a_ >= operand) ? registers_->SetCar() : registers_->ClearCar();
	(res == 0) ? registers_->SetZer() : registers_->ClearZer();
	CheckNegative(res) ? registers_->SetNeg() : registers_->ClearNeg();

	registers_->pc_ += pc_inc;

	return cycles;
}

unsigned Cpu::CPX(uint8_t opcode)
{
	uint8_t cycles = 0;
	uint8_t operand = 0x00;
	int8_t pc_inc = 0;
	uint16_t addr = 0x0000;

	switch (opcode)
	{
	case 0xe0:
		// Immediate
		Immediate(addr, pc_inc);
		cycles = 2;
		break;
	case 0xe4:
		// Zero Page
		ZeroPage(addr, pc_inc);
		cycles = 3;
		break;
	case 0xec:
		// Absolute
		Absolute(addr, pc_inc);
		cycles = 4;
		break;
	default:
		Logger::LogError("Unknown OPcode: 0x%02X at PC = 0x%04X", (unsigned)opcode, (unsigned)registers_->pc_);
		exit(EXIT_FAILURE);
	}

	operand = mmu_->ReadByte(addr);

	// Actual operation
	uint8_t res = registers_->x_ - operand;

	(registers_->x_ >= operand) ? registers_->SetCar() : registers_->ClearCar();
	(res == 0) ? registers_->SetZer() : registers_->ClearZer();
	CheckNegative(res) ? registers_->SetNeg() : registers_->ClearNeg();

	registers_->pc_ += pc_inc;

	return cycles;
}

unsigned Cpu::CPY(uint8_t opcode)
{
	uint8_t cycles = 0;
	uint8_t operand = 0x00;
	int8_t pc_inc = 0;
	uint16_t addr = 0x0000;

	switch (opcode)
	{
	case 0xc0:
		// Immediate
		Immediate(addr, pc_inc);
		cycles = 2;
		break;
	case 0xc4:
		// Zero Page
		ZeroPage(addr, pc_inc);
		cycles = 3;
		break;
	case 0xcc:
		// Absolute
		Absolute(addr, pc_inc);
		cycles = 4;
		break;
	default:
		Logger::LogError("Unknown OPcode: 0x%02X at PC = 0x%04X", (unsigned)opcode, (unsigned)registers_->pc_);
		exit(EXIT_FAILURE);
	}

	operand = mmu_->ReadByte(addr);

	// Actual operation
	uint8_t res = registers_->y_ - operand;

	(registers_->y_ >= operand) ? registers_->SetCar() : registers_->ClearCar();
	(res == 0) ? registers_->SetZer() : registers_->ClearZer();
	CheckNegative(res) ? registers_->SetNeg() : registers_->ClearNeg();

	registers_->pc_ += pc_inc;

	return cycles;
}

unsigned Cpu::INC(uint8_t opcode)
{
	uint8_t cycles = 0;
	uint8_t operand = 0x00;
	int8_t pc_inc = 0;
	uint16_t addr = 0x0000;

	switch (opcode)
	{
	case 0xe6:
		// Zero Page
		ZeroPage(addr, pc_inc);
		cycles = 5;
		break;
	case 0xf6:
		// Zero Page,X
		ZeroPageX(addr, pc_inc);
		cycles = 6;
		break;
	case 0xee:
		// Absolute
		Absolute(addr, pc_inc);
		cycles = 6;
		break;
	case 0xfe:
		// Absolute,X
		AbsoluteX(addr, pc_inc);
		cycles = 7;
		break;
	default:
		Logger::LogError("Unknown OPcode: 0x%02X at PC = 0x%04X", (unsigned)opcode, (unsigned)registers_->pc_);
		exit(EXIT_FAILURE);
	}

	operand = mmu_->ReadByte(addr);

	// Actual operation
	uint8_t res = operand + 1;

	(res == 0) ? registers_->SetZer() : registers_->ClearZer();
	CheckNegative(res) ? registers_->SetNeg() : registers_->ClearNeg();

	mmu_->WriteByte(addr, res);

	registers_->pc_ += pc_inc;

	return cycles;
}

unsigned Cpu::INX(uint8_t opcode)
{
	(void)opcode; //suppress warnings

	uint8_t res = registers_->x_ + 1;
	(res == 0) ? registers_->SetZer() : registers_->ClearZer();
	CheckNegative(res) ? registers_->SetNeg() : registers_->ClearNeg();

	registers_->x_ = res;
	registers_->pc_ += 1;

	return 2;
}

unsigned Cpu::INY(uint8_t opcode)
{
	(void)opcode; //suppress warnings

	uint8_t res = registers_->y_ + 1;
	(res == 0) ? registers_->SetZer() : registers_->ClearZer();
	CheckNegative(res) ? registers_->SetNeg() : registers_->ClearNeg();

	registers_->y_ = res;
	registers_->pc_ += 1;

	return 2;
}

unsigned Cpu::DEX(uint8_t opcode)
{
	(void)opcode; //suppress warnings

	uint8_t res = registers_->x_ - 1;
	(res == 0) ? registers_->SetZer() : registers_->ClearZer();
	CheckNegative(res) ? registers_->SetNeg() : registers_->ClearNeg();

	registers_->x_ = res;
	registers_->pc_ += 1;

	return 2;
}

unsigned Cpu::DEC(uint8_t opcode)
{
	uint8_t cycles = 0;
	uint8_t operand = 0x00;
	int8_t pc_inc = 0;
	uint16_t addr = 0x0000;

	switch (opcode)
	{
	case 0xc6:
		// Zero Page
		ZeroPage(addr, pc_inc);
		cycles = 5;
		break;
	case 0xd6:
		// Zero Page,X
		ZeroPageX(addr, pc_inc);
		cycles = 6;
		break;
	case 0xce:
		// Absolute
		Absolute(addr, pc_inc);
		cycles = 6;
		break;
	case 0xde:
		// Absolute,X
		AbsoluteX(addr, pc_inc);
		cycles = 7;
		break;
	default:
		Logger::LogError("Unknown OPcode: 0x%02X at PC = 0x%04X", (unsigned)opcode, (unsigned)registers_->pc_);
		exit(EXIT_FAILURE);
	}

	operand = mmu_->ReadByte(addr);

	// Actual operation
	uint8_t res = operand - 1;

	(res == 0) ? registers_->SetZer() : registers_->ClearZer();
	CheckNegative(res) ? registers_->SetNeg() : registers_->ClearNeg();

	mmu_->WriteByte(addr, res);

	registers_->pc_ += pc_inc;

	return cycles;
}

unsigned Cpu::DEY(uint8_t opcode)
{
	(void)opcode; //suppress warnings

	uint8_t res = registers_->y_ - 1;
	(res == 0) ? registers_->SetZer() : registers_->ClearZer();
	CheckNegative(res) ? registers_->SetNeg() : registers_->ClearNeg();

	registers_->y_ = res;
	registers_->pc_ += 1;

	return 2;
}

unsigned Cpu::ASL(uint8_t opcode)
{
	(void)opcode; //suppress warnings

	uint8_t cycles = 0;
	uint8_t operand = 0x00;
	uint16_t addr = 0x0000;
	int8_t pc_inc = 0;

	switch (opcode)
	{
	case 0x0a: {
		// Accumulator
		uint8_t accumulator = registers_->a_;
		((accumulator & 0x80) == 0x80) ? registers_->SetCar() : registers_->ClearCar();
		
		accumulator = accumulator << 1;
		
		(accumulator == 0) ? registers_->SetZer() : registers_->ClearZer();
		CheckNegative(accumulator) ? registers_->SetNeg() : registers_->ClearNeg();
		
		registers_->a_ = accumulator;
		registers_->pc_ += 1;
		return 2;
	}
	case 0x06:
		// Zero Page
		ZeroPage(addr, pc_inc);
		cycles = 5;
		break;
	case 0x16:
		// Zero Page,X
		ZeroPageX(addr, pc_inc);
		cycles = 6;
		break;
	case 0x0e:
		// Absolute
		Absolute(addr, pc_inc);
		cycles = 6;
		break;
	case 0x1e:
		//Absolute, X
		AbsoluteX(addr, pc_inc);
		cycles = 7;
		break;
	default:
		Logger::LogError("Unknown OPcode: 0x%02X at PC = 0x%04X", (unsigned)opcode, (unsigned)registers_->pc_);
		exit(EXIT_FAILURE);
	}

	operand = mmu_->ReadByte(addr);

	// Actual op

	((operand & 0x80) == 0x80) ? registers_->SetCar() : registers_->ClearCar();

	uint8_t res = operand << 1;

	(res == 0) ? registers_->SetZer() : registers_->ClearZer();
	CheckNegative(res) ? registers_->SetNeg() : registers_->ClearNeg();

	mmu_->WriteByte(addr, res); // Write back result

	registers_->pc_ += pc_inc;

	return cycles;
}

unsigned Cpu::LSR(uint8_t opcode)
{
	uint8_t cycles = 0;
	uint8_t operand = 0x00;
	uint16_t addr = 0x0000;
	int8_t pc_inc = 0;


	switch (opcode)
	{
	case 0x4a: {
		// Accumulator
		uint8_t accumulator = registers_->a_;
		((accumulator & 0x01) == 0x01) ? registers_->SetCar() : registers_->ClearCar();
		
		accumulator = accumulator >> 1;
		
		(accumulator == 0) ? registers_->SetZer() : registers_->ClearZer();
		CheckNegative(accumulator) ? registers_->SetNeg() : registers_->ClearNeg();
		
		registers_->a_ = accumulator;
		registers_->pc_ += 1;
		return 2;
	}
	case 0x46:
		// Zero Page
		ZeroPage(addr, pc_inc);
		cycles = 5;
		break;
	case 0x56:
		// Zero Page,X
		ZeroPageX(addr, pc_inc);
		cycles = 6;
		break;
	case 0x4e:
		// Absolute
		Absolute(addr, pc_inc);
		cycles = 6;
		break;
	case 0x5e:
		//Absolute, X
		AbsoluteX(addr, pc_inc);
		cycles = 7;
		break;
	default:
		Logger::LogError("Unknown OPcode: 0x%02X at PC = 0x%04X", (unsigned)opcode, (unsigned)registers_->pc_);
		exit(EXIT_FAILURE);
	}

	operand = mmu_->ReadByte(addr);

	// Actual op

	((operand & 0x01) == 0x01) ? registers_->SetCar() : registers_->ClearCar();

	uint8_t res = operand >> 1;

	(res == 0) ? registers_->SetZer() : registers_->ClearZer();
	CheckNegative(res) ? registers_->SetNeg() : registers_->ClearNeg();

	mmu_->WriteByte(addr, res); // Write back result

	registers_->pc_ += pc_inc;

	return cycles;
}

unsigned Cpu::ROL(uint8_t opcode)
{
	/*Move each of the bits in either A or M one place to the left. 
	Bit 0 is filled with the current value of the carry flag whilst 
	the old bit 7 becomes the new carry flag value.*/

	uint8_t cycles = 0;
	uint8_t operand = 0x00;
	uint16_t addr = 0x0000;
	int8_t pc_inc = 0;
	uint8_t lsb = 0x00;


	switch (opcode)
	{
	case 0x2a: {
		// Accumulator
		uint8_t accumulator = registers_->a_;
		registers_->CheckCar() ? lsb = 0x01 : lsb = 0x00;
		((accumulator & 0x80) == 0x80) ? registers_->SetCar() : registers_->ClearCar();
		
		accumulator = accumulator << 1;
		accumulator |= lsb;

		(accumulator == 0) ? registers_->SetZer() : registers_->ClearZer();
		CheckNegative(accumulator) ? registers_->SetNeg() : registers_->ClearNeg();
		
		registers_->a_ = accumulator;
		registers_->pc_ += 1;
		return 2;
	}
	case 0x26:
		// Zero Page
		ZeroPage(addr, pc_inc);
		cycles = 5;
		break;
	case 0x36:
		// Zero Page,X
		ZeroPageX(addr, pc_inc);
		cycles = 6;
		break;
	case 0x2e:
		// Absolute
		Absolute(addr, pc_inc);
		cycles = 6;
		break;
	case 0x3e:
		//Absolute, X
		AbsoluteX(addr, pc_inc);
		cycles = 7;
		break;
	default:
		Logger::LogError("Unknown OPcode: 0x%02X at PC = 0x%04X", (unsigned)opcode, (unsigned)registers_->pc_);
		exit(EXIT_FAILURE);
	}

	operand = mmu_->ReadByte(addr);

	// Actual op

	registers_->CheckCar() ? lsb = 0x01 : lsb = 0x00;
	((operand & 0x80) == 0x80) ? registers_->SetCar() : registers_->ClearCar();

	uint8_t res = operand << 1;
	res |= lsb;

	(res == 0) ? registers_->SetZer() : registers_->ClearZer();
	CheckNegative(res) ? registers_->SetNeg() : registers_->ClearNeg();

	mmu_->WriteByte(addr, res); // Write back result

	registers_->pc_ += pc_inc;

	return cycles;
}

unsigned Cpu::ROR(uint8_t opcode)
{
	/*Move each of the bits in either A or M one place to the right. 
	Bit 7 is filled with the current value of the carry flag whilst 
	the old bit 0 becomes the new carry flag value.*/

	uint8_t cycles = 0;
	uint8_t operand = 0x00;
	uint16_t addr = 0x0000;
	int8_t pc_inc = 0;
	uint8_t msb = 0x00;


	switch (opcode)
	{
	case 0x6a: {
		// Accumulator
		uint8_t accumulator = registers_->a_;
		registers_->CheckCar() ? msb = 0x80 : msb = 0x00;
		((accumulator & 0x01) == 0x01) ? registers_->SetCar() : registers_->ClearCar();

		accumulator = accumulator >> 1;
		accumulator |= msb;

		(accumulator == 0) ? registers_->SetZer() : registers_->ClearZer();
		CheckNegative(accumulator) ? registers_->SetNeg() : registers_->ClearNeg();

		registers_->a_ = accumulator;
		registers_->pc_ += 1;
		return 2;
	}
	case 0x66:
		// Zero Page
		ZeroPage(addr, pc_inc);
		cycles = 5;
		break;
	case 0x76:
		// Zero Page,X
		ZeroPageX(addr, pc_inc);
		cycles = 6;
		break;
	case 0x6e:
		// Absolute
		Absolute(addr, pc_inc);
		cycles = 6;
		break;
	case 0x7e:
		//Absolute, X
		AbsoluteX(addr, pc_inc);
		cycles = 7;
		break;
	default:
		Logger::LogError("Unknown OPcode: 0x%02X at PC = 0x%04X", (unsigned)opcode, (unsigned)registers_->pc_);
		exit(EXIT_FAILURE);
	}

	operand = mmu_->ReadByte(addr);

	// Actual op

	registers_->CheckCar() ? msb = 0x80 : msb = 0x00;
	((operand & 0x01) == 0x01) ? registers_->SetCar() : registers_->ClearCar();

	uint8_t res = operand >> 1;
	res |= msb;

	(res == 0) ? registers_->SetZer() : registers_->ClearZer();
	CheckNegative(res) ? registers_->SetNeg() : registers_->ClearNeg();

	mmu_->WriteByte(addr, res); // Write back result

	registers_->pc_ += pc_inc;

	return cycles;
}

unsigned Cpu::JMP(uint8_t opcode)
{
	uint16_t indirect_addr = 0x0000;
	uint16_t absolute_addr = 0x0000;

	switch (opcode)
	{
	case 0x4c: 
	// absolute
		absolute_addr = mmu_->ReadWord(registers_->pc_ + 1);

		registers_->pc_ = absolute_addr;

		return 3;

	case 0x6c: {
		// indirect
		/*
		Here there is an Hardware bug in the 6502
		On the page barrier (xxFF) the least significant byte is ignored.
		So the address of the jump wraps araound

		AN INDIRECT JUMP MUST NEVER USE A
		VECTOR BEGINNING ON THE LAST BYTE
		OF A PAGE

		For example if address $3000 contains $40, $30FF contains $80,
		and $3100 contains $50, the result of JMP ($30FF) will be a 
		transfer of control to $4080 rather than $5080 as you intended 
		i.e. the 6502 took the low byte of the address from $30FF and 
		the high byte from $3000. 
		*/
		uint16_t indirect_addr_lo = mmu_->ReadByte(registers_->pc_ + 1);
		uint16_t indirect_addr_hi = mmu_->ReadByte(registers_->pc_ + 2);

		indirect_addr = (indirect_addr_hi << 8) | indirect_addr_lo;

		uint16_t absolute_addr_lo = mmu_->ReadByte(indirect_addr);

		/*
		Recompute another indirect address after possible wrapping
		*/

		indirect_addr_lo = 0xff & (indirect_addr_lo + 1);
		indirect_addr = (indirect_addr_hi << 8) | indirect_addr_lo;
		
		uint16_t absolute_addr_hi = mmu_->ReadByte(indirect_addr);

		absolute_addr = (absolute_addr_hi << 8) | absolute_addr_lo;

		registers_->pc_ = absolute_addr;
		
		return 5;
	}
	default:
		Logger::LogError("Unknown OPcode: 0x%02X at PC = 0x%04X", (unsigned)opcode, (unsigned)registers_->pc_);
		exit(EXIT_FAILURE);
	}
}

unsigned Cpu::JSR(uint8_t opcode)
{
	(void)opcode; //suppress warnings

	/*JSR pushes the address-1 of the next operation on to the stack 
	before transferring program control to the following address. 
	Subroutines are normally terminated by a RTS op code. */

	uint16_t absolute_addr = mmu_->ReadWord(registers_->pc_ + 1);

	// next op after return from subroutine address-1 
	uint16_t next_op_addr = registers_->pc_ + 2;

	// push onto stack
	mmu_->WriteByte((uint16_t)registers_->s_ + STACK_BASE, (uint8_t)(next_op_addr >> 8));
	mmu_->WriteByte((uint16_t)registers_->s_ + STACK_BASE - 1, (uint8_t)(next_op_addr & 0x00ff));

	registers_->s_ -= 2;

	// actual jump
	registers_->pc_ = absolute_addr;

	return 6;
}

unsigned Cpu::RTS(uint8_t opcode)
{
	(void)opcode; //suppress warnings

	/*RTS pulls the top two bytes off the stack (low byte first) 
	and transfers program control to that address+1. It is used, 
	as expected, to exit a subroutine invoked via JSR which pushed 
	the address-1. */

	// pop from stack
	registers_->s_ += 2;

	uint16_t next_op_addr = (uint16_t)mmu_->ReadByte((uint16_t)registers_->s_ + STACK_BASE) << 8;
	next_op_addr |= (uint16_t)mmu_->ReadByte((uint16_t)registers_->s_ + STACK_BASE - 1);
	
	// actual jump
	registers_->pc_ = next_op_addr + 1;

	return 6;
}

unsigned Cpu::BCC(uint8_t opcode)
{
	(void)opcode; //suppress warnings

	int8_t pc_inc = 0x00;
	uint8_t cycles = 2;

	if (!registers_->CheckCar()) {
		//if branch taken
		if (Relative(pc_inc)) {
			// if page crossed
			cycles++;
		}
		cycles++;
	}
	// both if branch not taken or branch taken the PC is further incremented by 2
	pc_inc += 2;

	registers_->pc_ += pc_inc;
	
	return cycles;
}

unsigned Cpu::BCS(uint8_t opcode)
{
	(void)opcode; //suppress warnings

	int8_t pc_inc = 0x00;
	uint8_t cycles = 2;

	if (registers_->CheckCar()) {
		// branch taken
		if (Relative(pc_inc)) {
			// page crossed
			cycles++;
		}
		cycles++;
	}
	// both if branch not taken or branch taken the PC is further incremented by 2
	pc_inc += 2;

	registers_->pc_ += pc_inc;

	return cycles;
}

unsigned Cpu::BEQ(uint8_t opcode)
{
	(void)opcode; //suppress warnings

	int8_t pc_inc = 0x00;
	uint8_t cycles = 2;

	if (registers_->CheckZer()) {
		// branch taken
		if (Relative(pc_inc)) {
			// page crossed
			cycles++;
		}
		cycles++;
	}
	// both if branch not taken or branch taken the PC is further incremented by 2
	pc_inc += 2;

	registers_->pc_ += pc_inc;

	return cycles;
}

unsigned Cpu::BMI(uint8_t opcode)
{
	(void)opcode; //suppress warnings

	int8_t pc_inc = 0x00;
	uint8_t cycles = 2;

	if (registers_->CheckNeg()) {
		// branch taken
		if (Relative(pc_inc)) {
			// page crossed
			cycles++;
		}
		cycles++;
	}
	// both if branch not taken or branch taken the PC is further incremented by 2
	pc_inc += 2;

	registers_->pc_ += pc_inc;

	return cycles;
}

unsigned Cpu::BNE(uint8_t opcode)
{
	(void)opcode; //suppress warnings

	int8_t pc_inc = 0x00;
	uint8_t cycles = 2;

	if (!registers_->CheckZer()) {
		// branch taken
		if (Relative(pc_inc)) {
			// page crossed
			cycles++;
		}
		cycles++;
	}
	// both if branch not taken or branch taken the PC is further incremented by 2
	pc_inc += 2;

	registers_->pc_ += pc_inc;

	return cycles;
}

unsigned Cpu::BPL(uint8_t opcode)
{
	(void)opcode; //suppress warnings

	int8_t pc_inc = 0x00;
	uint8_t cycles = 2;

	if (!registers_->CheckNeg()) {
		// branch taken
		if (Relative(pc_inc)) {
			// page crossed
			cycles++;
		}
		cycles++;
	}
	// both if branch not taken or branch taken the PC is further incremented by 2
	pc_inc += 2;

	registers_->pc_ += pc_inc;

	return cycles;
}

unsigned Cpu::BVC(uint8_t opcode)
{
	(void)opcode; //suppress warnings

	int8_t pc_inc = 0x00;
	uint8_t cycles = 2;

	if (!registers_->CheckOvf()) {
		// branch taken
		if (Relative(pc_inc)) {
			// page crossed
			cycles++;
		}
		cycles++;
	}
	// both if branch not taken or branch taken the PC is further incremented by 2
	pc_inc += 2;

	registers_->pc_ += pc_inc;

	return cycles;
}

unsigned Cpu::BVS(uint8_t opcode)
{
	(void)opcode; //suppress warnings

	int8_t pc_inc = 0x00;
	uint8_t cycles = 2;

	if (registers_->CheckOvf()) {
		// branch taken
		if (Relative(pc_inc)) {
			// page crossed
			cycles++;
		}
		cycles++;
	}
	// both if branch not taken or branch taken the PC is further incremented by 2
	pc_inc += 2;

	registers_->pc_ += pc_inc;

	return cycles;
}

unsigned Cpu::CLC(uint8_t opcode)
{
	(void)opcode; //suppress warnings

	registers_->ClearCar();

	registers_->pc_ += 1;

	return 2;
}

unsigned Cpu::CLD(uint8_t opcode)
{
	(void)opcode; //suppress warnings

	registers_->ClearDec();

	registers_->pc_ += 1;

	return 2;
}

unsigned Cpu::CLI(uint8_t opcode)
{
	(void)opcode; //suppress warnings

	registers_->ClearIrd();

	registers_->pc_ += 1;

	return 2;
}

unsigned Cpu::CLV(uint8_t opcode)
{
	(void)opcode; //suppress warnings

	registers_->ClearOvf();

	registers_->pc_ += 1;

	return 2;
}

unsigned Cpu::SEC(uint8_t opcode)
{
	(void)opcode; //suppress warnings

	registers_->SetCar();

	registers_->pc_ += 1;

	return 2;
}

unsigned Cpu::SED(uint8_t opcode)
{
	(void)opcode; //suppress warnings

	registers_->SetDec();

	registers_->pc_ += 1;

	return 2;
}

unsigned Cpu::SEI(uint8_t opcode)
{
	(void)opcode; //suppress warnings

	registers_->SetIrd();

	registers_->pc_ += 1;

	return 2;
}

unsigned Cpu::BRK(uint8_t opcode)
{
	(void)opcode; //suppress warnings

	/*The BRK instruction forces the generation of an interrupt request. 
	The program counter + 2 and processor status are pushed on the stack then 
	the IRQ interrupt vector at $FFFE/F is loaded into the PC and the 
	break flag in the status set to one.*/

	registers_->pc_ += 2;

	// push pc
	mmu_->WriteByte((uint16_t)registers_->s_ + STACK_BASE, (uint8_t)(registers_->pc_ >> 8));
	mmu_->WriteByte((uint16_t)registers_->s_ + STACK_BASE - 1, (uint8_t)(registers_->pc_ & 0x00ff));
	registers_->s_ -= 2;

	// push p with B at 1
	mmu_->WriteByte((uint16_t)registers_->s_ + STACK_BASE, registers_->p_ | 0b00110000);
	registers_->s_ -= 1;
	registers_->SetIrd();

	// load IRQ interrupt vector
	registers_->pc_ = mmu_->ReadWord(IRQ_BASE);

	return 7;
}

unsigned Cpu::NOP(uint8_t opcode)
{
	(void)opcode; //suppress warnings

	registers_->pc_ += 1;

	return 2;
}

unsigned Cpu::RTI(uint8_t opcode)
{
	(void)opcode; //suppress warnings

	/*The RTI instruction is used at the end of an interrupt processing 
	routine. It pulls the processor flags from the stack followed by 
	the program counter.*/

	// pop p
	registers_->s_ += 1;
	uint8_t operand = mmu_->ReadByte((uint16_t)registers_->s_ + STACK_BASE);
	CheckBit(operand, NEG) ? registers_->SetNeg() : registers_->ClearNeg();
	CheckBit(operand, OVF) ? registers_->SetOvf() : registers_->ClearOvf();
	CheckBit(operand, DEM) ? registers_->SetDec() : registers_->ClearDec();
	CheckBit(operand, IRQ) ? registers_->SetIrd() : registers_->ClearIrd();
	CheckBit(operand, ZER) ? registers_->SetZer() : registers_->ClearZer();
	CheckBit(operand, CAR) ? registers_->SetCar() : registers_->ClearCar();

	// pop pc
	registers_->s_ += 2;
	registers_->pc_ = (uint16_t)mmu_->ReadByte((uint16_t)registers_->s_ + STACK_BASE) << 8;
	registers_->pc_ |= (uint16_t)mmu_->ReadByte((uint16_t)registers_->s_ + STACK_BASE - 1);

	return 6;
}

inline bool Cpu::Immediate(uint16_t& addr, int8_t& pc_inc)
{
	addr = registers_->pc_ + 1;
	
	pc_inc = 2;

	return false;
}

inline bool Cpu::ZeroPage(uint16_t& addr, int8_t& pc_inc)
{
	uint8_t zero_page_addr = mmu_->ReadByte(registers_->pc_ + 1); 
	addr = (uint16_t)zero_page_addr;

	pc_inc = 2;

	return false;
}

inline bool Cpu::ZeroPageX(uint16_t& addr, int8_t& pc_inc)
{
	uint8_t zero_page_addr = mmu_->ReadByte(registers_->pc_ + 1) + registers_->x_; // Wraps around if > 255
	addr = (uint16_t)zero_page_addr;

	pc_inc = 2;

	return false;
}

inline bool Cpu::ZeroPageY(uint16_t& addr, int8_t& pc_inc)
{
	uint8_t zero_page_addr = mmu_->ReadByte(registers_->pc_ + 1) + registers_->y_; // Wraps around if > 255
	addr = (uint16_t)zero_page_addr;

	pc_inc = 2;

	return false;
}

inline bool Cpu::Absolute(uint16_t& addr, int8_t& pc_inc)
{
	addr = mmu_->ReadWord(registers_->pc_ + 1);

	pc_inc = 3;

	return false;
}

inline bool Cpu::IndexedIndirect(uint16_t& addr, int8_t& pc_inc)
{
	uint8_t operand = mmu_->ReadByte(registers_->pc_ + 1);
	uint16_t addr_lo = (uint16_t)mmu_->ReadByte(0xff & (registers_->x_ + operand)); 
	/* 0xff & is used to force the wrapping. Otherwise the funct param is read as a uint16_t and not wraps*/
	uint16_t addr_hi = (uint16_t)mmu_->ReadByte(0xff & (registers_->x_ + operand + 1)); 
	addr = (addr_hi << 8) | addr_lo;

	pc_inc = 2;

	return false;
}

inline bool Cpu::AbsoluteX(uint16_t& addr, int8_t& pc_inc)
{
	addr = mmu_->ReadWord(registers_->pc_ + 1) + (uint16_t)registers_->x_;

	pc_inc = 3;

	return CheckPageCrossed(addr, registers_->x_);
}

inline bool Cpu::AbsoluteY(uint16_t& addr, int8_t& pc_inc)
{
	addr = mmu_->ReadWord(registers_->pc_ + 1) + (uint16_t)registers_->y_;

	pc_inc = 3;

	return CheckPageCrossed(addr, registers_->y_);
}

inline bool Cpu::IndirectIndexed(uint16_t& addr, int8_t& pc_inc)
{
	uint8_t operand = mmu_->ReadByte(registers_->pc_ + 1);
	uint16_t addr_lo = (uint16_t)mmu_->ReadByte(operand);
	uint16_t addr_hi = (uint16_t)mmu_->ReadByte(0xff & (operand + 1));
	addr = ((addr_hi << 8) | addr_lo) + (uint16_t)registers_->y_;
	pc_inc = 2;

	return CheckPageCrossed(addr, registers_->y_);
}

inline bool Cpu::Relative(int8_t& pc_inc)
{
	pc_inc = mmu_->ReadByte(registers_->pc_ + 1);

	return CheckPageCrossed(registers_->pc_, pc_inc);
}
