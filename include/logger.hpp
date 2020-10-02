#pragma once
#include <iostream>
#include <stdarg.h>
#include "pch.hpp"

class Logger
{
public:

	static void Log(const char* message, ...);
	static void LogError(const char* message, ...);
	static void Enable();
	static void Disable();

private:
	static bool is_enabled_; 
	Logger() {};
};

