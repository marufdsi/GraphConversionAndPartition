TARGET := partition

CXX = icpc

CXXFLAGS = -O3 -std=c++11 -Wall

LIBS = -lpthread


SOURCES = $(wildcard *.cpp) $(wildcard */*.cpp) $(wildcard */*/*.cpp)

DEPS = $(wildcard *.h) $(wildcard */*.h) $(wildcard */*/*.h)

Partition_OBJ := $(SOURCES:.cpp=.o)


.PHONY: all

all: $(TARGET)


$(TARGET): $(Partition_OBJ) $(SOURCES)
	$(CXX) $(Partition_OBJ) $(CXXFLAGS) $(LIBS) -o $@

clean:
	rm -r $(TARGET) $(Partition_OBJ)

