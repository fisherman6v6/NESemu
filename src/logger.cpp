#include "logger.hpp"

bool Logger::is_enabled_ = true;
std::ofstream Logger::log_file;

void Logger::Enable() {
	if (!log_file.is_open()) {
		log_file.open(LOGFILE, std::ofstream::out);
	}
	is_enabled_ = 1;
}

void Logger::Disable() {
	if (log_file.is_open()) {
		log_file.close();
	}
	
	is_enabled_ = 0;
}

void Logger::LogCpuRegisters(const std::unique_ptr<Registers>& registers)
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

void Logger::LogOp(const std::unique_ptr<Registers>& registers, const std::unique_ptr<Mmu>& mmu, uint64_t cyc) {

	if (!is_enabled_) {
		return;
	}

	if (!log_file) {
		std::cout << "Log File ERROR" << std::endl;
		return;
	}

	log_file << std::hex << std::setfill('0') << std::setw(2) << std::uppercase << std::hex << (unsigned)registers->getPC() << "    " 
		<< std::setfill('0') << std::setw(2) << (unsigned)mmu->ReadByte(registers->getPC()) << "    "
		" A:" << std::setfill('0') << std::setw(2) <<std::hex << (unsigned)registers->getA() << 
		" X:" << std::setfill('0') << std::setw(2) <<std::hex << (unsigned)registers->getX() << 
		" Y:" << std::setfill('0') << std::setw(2) <<std::hex << (unsigned)registers->getY() << 
		" P:" << std::setfill('0') << std::setw(2) <<std::hex << (unsigned)registers->getP() << 
		" S:" << std::setfill('0') << std::setw(2) <<std::hex << (unsigned)registers->getS() <<
		" CYC:" << std::dec << (unsigned long int)cyc << std::endl;
}

void Logger::LogMemory(const std::unique_ptr<Mmu>& memory, size_t start, size_t size, const char* tag)
{
	if (!is_enabled_) {
		return;
	}

	printf("\n- - - - - - - - - - - - - - - - - - - - - - - - - -\n");
    uint16_t end = 0;
	if ((start + size) > 0xffff) {
	    end = 0xffff;
	}
	else {
	    end = start + size;
	}
	if (tag != nullptr) {
		std::cout << tag << std::endl;
	}
	for (auto i = start; i <= end; i += 16) {
		printf("%04x: ", i);
		for (auto j = i; (j < i + 16) && (j <= end); j++) {
			printf("%02x ", (unsigned)memory->ReadByte(j));
		}
		std::cout << std::endl;
	}
	std::cout << "\n- - - - - - - - - - - - - - - - - - - - - - - - - -\n\n" << std::endl;
}

void Logger::DebugHelp()
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
