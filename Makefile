CXX		  := g++
CXX_FLAGS := -Wall -Wextra -std=c++17 -ggdb -g3

BIN		:= bin
SRC		:= src
INCLUDE	:= include
LIB		:= lib

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
	$(CXX) -c -o $@ $< $(CXXFLAGS) -I$(INCLUDE)
    
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(LIBRARIES) $(OBJECTS) -o $@

.PHONY: clean

default: $(TARGET)

clean:
	-rm $(BIN)/*;	\
	rm $(OBJ)/*;	\
	rm $(SRC)/*.o
