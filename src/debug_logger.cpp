#include "debug_logger.hpp"

bool DebugLogger::is_enabled_ = true;
std::ofstream DebugLogger::log_file_;
std::string DebugLogger::log_file_path_ = LOGFILE;

DebugLogger::~DebugLogger() = default;

void DebugLogger::LogCpuRegisters(const Registers* registers)
{
	if (!is_enabled_) {
		return;
	}

	printf("\n- - - - - - - - - - - - - - - - - - - - - - - - - -\n");
	printf("PC = %02x\n", (unsigned)registers->getPC());
	printf("A = %02x\n", (unsigned)registers->getA());
	printf("X = %02x\n", (unsigned)registers->getX());
	printf("Y = %02x\n", (unsigned)registers->getY());
	printf("P = %02x\n", (unsigned)registers->getP());
	printf("S = %02x\n", (unsigned)registers->getS());
	printf("FLAGS = N V - B D I Z C\n");
	printf("        %u %u - %u %u %u %u %u", (unsigned)registers->CheckNeg(), (unsigned)registers->CheckOvf(), (unsigned)registers->CheckBrk(), 
											(unsigned)registers->CheckDec(), (unsigned)registers->CheckIrd(), (unsigned)registers->CheckZer(), 
											(unsigned)registers->CheckCar());

	std::cout << "\n- - - - - - - - - - - - - - - - - - - - - - - - - -\n\n" << std::endl;

}

void DebugLogger::FileLogOp(const Registers* registers, const Mmu* mmu, uint64_t cyc) {

	if (!is_enabled_) {
		return;
	}

	/* Log file is cleared the first time it's opened*/
	static bool first_time = true;

	if (first_time) {
		log_file_.open(LOGFILE, std::ofstream::out);
		first_time = false;
	}

	else {
		log_file_.open(LOGFILE, std::ofstream::app);
	}

	if (!log_file_) {
		std::cout << "Log File ERROR" << std::endl;
		return;
	}

	log_file_ << std::setfill('0') << std::setw(4) << std::uppercase << std::hex << (unsigned)registers->getPC() << "    " 
		<< std::setfill('0') << std::setw(2) << (unsigned)mmu->ReadByte(registers->getPC()) << "    "
		" A:" << std::setfill('0') << std::setw(2) << std::hex << (unsigned)registers->getA() << 
		" X:" << std::setfill('0') << std::setw(2) << std::hex << (unsigned)registers->getX() << 
		" Y:" << std::setfill('0') << std::setw(2) << std::hex << (unsigned)registers->getY() << 
		" P:" << std::setfill('0') << std::setw(2) << std::hex << (unsigned)registers->getP() << 
		" S:" << std::setfill('0') << std::setw(2) << std::hex << (unsigned)registers->getS() <<
		" CYC:" << std::dec << (unsigned long int)cyc << std::endl;

	log_file_.close();
}

void DebugLogger::LogMemory(const Mmu* memory, size_t start, size_t size, const char* tag)
{
	if (!is_enabled_) {
		return;
	}

	printf("\n- - - - - - - - - - - - - - - - - - - - - - - - - -\n");
    uint16_t end = 0;
	if ((start + size - 1) > 0xffff) {
	    end = 0xffff;
	}
	else {
	    end = start + size - 1;
	}
	if (tag != nullptr) {
		std::cout << tag << std::endl;
	}
	for (unsigned i = start; i <= end; i += 16) {
		printf("%04x: ", i);
		for (unsigned j = i; (j < i + 16) && (j <= end); j++) {
			printf("%02x ", (unsigned)memory->ReadByte(j));
		}
		std::cout << std::endl;
	}
	std::cout << "\n- - - - - - - - - - - - - - - - - - - - - - - - - -\n\n" << std::endl;
}

void DebugLogger::DebugHelp()
{
	std::cout << "Debug commands:" << "\n";
	std::cout << "\
				s: step\n \
				b <program counter value> : set breakpoint at <program counter value>\n \
				p <regs> : print registers\n \
				p <mem> <start address> <displacement> : print memory block\n \
				c : continue until breakpoint\n \
				q : quit";
	std::cout << "\n- - - - - - - - - - - - - - - - - - - - - - - - - -\n\n" << std::endl;
}
