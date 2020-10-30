cmake_minimum_required(VERSION 3.17)
project(NESemu)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(include)
include_directories(src)

set(SFML_INCLUDE_DIR "/usr/include/SFML")
set(SFML_LIBRARY_DIR "/usr/lib")
set(SFML_DIR "/usr/lib/cmake/SFML/")

link_directories(SFML_LIBRARY_DIR)
include_directories(SFML_INCLUDE_DIR)

find_package(SFML 2.5.1 COMPONENTS system window graphics network audio)

if(SFML_FOUND)
    message(STATUS "SFML_INCLUDE_DIR: ${SFML_INCLUDE_DIR}")
    message(STATUS "SFML_LIBRARIES: ${SFML_LIBRARIES}")
    message(STATUS "SFML_VERSION: ${SFML_VERSION}")
endif()

add_executable(NESemu
        include/cartridge.hpp
        include/cpu.hpp
        include/debug_logger.hpp
        include/debugger.hpp
        include/emulator.hpp
        include/fps_counter.hpp
        include/i_mapper.hpp
        include/i_memory_unit.hpp
        include/logger.hpp
        include/mapper_000.hpp
        include/mmu.hpp
        include/pch.hpp
        include/ppu.hpp
        include/registers.hpp
        src/cartridge.cpp
        src/cpu.cpp
        src/debug_logger.cpp
        src/debugger.cpp
        src/emulator.cpp
        src/fps_counter.cpp
        src/logger.cpp
        src/main.cpp
        src/mapper_000.cpp
        src/mmu.cpp
        src/ppu.cpp
        src/registers.cpp src/args_queue.cpp include/args_queue.h)

target_link_libraries (NESemu sfml-graphics sfml-window sfml-system)