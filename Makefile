CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
TARGET = main

SRCS = main.cpp algos.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

clean:
	rm -f $(OBJS) $(TARGET)

algos.o: algos.cpp algos.h
main.o: main.cpp algos.h