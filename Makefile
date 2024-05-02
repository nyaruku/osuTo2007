# Makefile for Windows

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
