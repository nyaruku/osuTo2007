# osuV3
Convert any osu beatmap to v3 (playable in the 2007 client)

Compiling: Makefile
```make
# Makefile for Windows

# Compiler
CXX = g++

CXXFLAGS = -m32 -Wall -std=c++20 -O3

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

