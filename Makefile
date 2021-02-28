CC	= gcc
CFLAGS=-g -c -w -m32
CCXX	= g++
CXXFLAGS=-g -c -w -m32
LDFLAGS= -L./zlib -lstdc++ -lc -lm -lSDL -lz -m32
CSOURCES=files.c dirent.c fpattern.c
SOURCES=assoc.cpp events.cpp float.cpp imports.cpp main.cpp misc.cpp movies.cpp signal.cpp sounds.cpp
OBJECTS= $(CSOURCES:.c=.o) $(SOURCES:.cpp=.o) fallout2.o
EXECUTABLE=fallout2

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) $(SOURCES) $(CSOURCES) *.h
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)
	
fallout2.o: asm/*.asm
	yasm -f elf32 -g DWARF2 asm/fallout2.asm -o fallout2.o

.cpp.o:
	$(CXX) $(CXXFLAGS) $< -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	@-$(RM) *.o Fallout2 2>/dev/null
