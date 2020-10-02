#include "logger.hpp"

bool Logger::is_enabled_ = true;

void Logger::Enable() {
	/*if (!log_file.is_open()) {
		log_file.open(LOGFILE, std::ofstream::out);
	}*/
	is_enabled_ = 1;
}

void Logger::Disable() {
	/*if (log_file.is_open()) {
		log_file.close();
	}*/
	
	is_enabled_ = 0;
}

void Logger::Log(const char* message, ...) {
	if (!is_enabled_) {
        return;
    }

    va_list argPointer;
    va_start(argPointer, message);

    char buffer[1024];
    std::vsnprintf(buffer, ARRAYSIZE(buffer), message, argPointer);

    va_end(argPointer);
    std::cout << buffer << std::endl;
}

void Logger::LogError(const char* message, ...) {

	va_list argPointer;
    va_start(argPointer, message);

    char buffer[1024];
    std::vsnprintf(buffer, ARRAYSIZE(buffer), message, argPointer);

    va_end(argPointer);
    std::cerr << buffer << std::endl;
}