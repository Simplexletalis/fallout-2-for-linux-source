CC	= gcc
CFLAGS=-std=c++11 -g -c -w -m32
LDFLAGS= -lstdc++ -lc -lm -lSDL -m32 
SOURCES=assoc.cpp dir.cpp Events.cpp files.cpp float.cpp imports.cpp main.cpp misc.cpp movies.cpp signal.cpp sounds.cpp
OBJECTS= $(SOURCES:.cpp=.o) fallout2.o
EXECUTABLE=Fallout2

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)
	
fallout2.o: 
	yasm -f elf32 -g DWARF2 asm/fallout2.asm -o fallout2.o

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
