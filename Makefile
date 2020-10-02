CXX		  := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -ggdb3 -g3

BIN		:= bin
SRC		:= src
INCLUDE	:= include
LIB		:= lib
#DEF 	:= -DALT_PC=0xc004

LIBRARIES	:= -lsfml-graphics -lsfml-window -lsfml-system
EXECUTABLE	:= main.out
OBJ := obj

SOURCES  := $(wildcard src/*.cpp)
_OBJECTS := $(patsubst %.cpp, %.o, $(SOURCES))
OBJECTS := $(subst ..src/,,$(_OBJECTS))
HEADERS := $(wildcard $(INCLUDE)/*.h)
TARGET := $(BIN)/$(EXECUTABLE)

all: $(TARGET)

run: clean all
	clear
	./$(TARGET)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<  -I$(INCLUDE)
    
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(LIBRARIES) $(OBJECTS) -o $@

.PHONY: clean

default: $(TARGET)

clean:
	-rm $(BIN)/*;	\
	rm $(OBJ)/*;	\
	rm $(SRC)/*.o
