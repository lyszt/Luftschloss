CXX      := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -Wpedantic -g
TARGET   := luftschloss_core
SRCS     := main.cpp
OBJS     := $(SRCS:.cpp=.o)

.PHONY: all run clean cmake

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

run: $(TARGET)
	./$(TARGET)

clean:
	$(RM) $(TARGET) $(OBJS)

# Out-of-source CMake build into build/
cmake:
	cmake -S . -B build
	cmake --build build
