# osuTo2007
Convert any osu beatmap to v3 (playable in the 2007 client)

__Download to Executable will be provided once a proper working version does exist.__

Compiling: Makefile
```make
# Compiler
CXX = g++

CXXFLAGS = -Wall -std=c++20 -O3

SOURCES = $(wildcard *.cpp)
OBJS = $(SOURCES:.cpp=.o)

# Output
EXEC = osuTo2007.exe  

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^
	

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
```

