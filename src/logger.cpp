#include "logger.hpp"

bool Logger::is_enabled_ = true;

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

void Logger::LogMemory(const std::unique_ptr<Mmu>& memory, uint16_t start, uint16_t size, const char* tag)
{
	if (!is_enabled_) {
		return;
	}

	printf("\n- - - - - - - - - - - - - - - - - - - - - - - - - -\n");

	uint16_t end = start + size;
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
