# osuTo2007
Convert any osu beatmap to v3 (playable in the 2007 client)

# What does it do?
- Restore v3 beatmap structure
- Handling SV points by multiplying with BPM (bc 2007 doesnt support SV) -> SV Point to Timing Point
- Fix Slider Start Position
- TODO: represent modern slider shapes in v3

# Performance Issues within 2007 client
- Beatmaps with a lot of timing points/objects will take long time to load when u play__
  -> each 100KB ~ 5-6minute load time__
  -> lag spikes/unstable frame rate__

Download from release, or compile:

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

