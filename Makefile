CXX=g++
CPPFLAGS=-std=c++17 -stdlib=libc++ -O3 -mavx2

# Correct openMP flags for macos/other
OS := $(shell uname)
ifeq ($(OS), Darwin)
	CPPFLAGS += -lomp -Xpreprocessor -fopenmp
else
	CPPFLAGS += -fopenmp
endif

SRC=./src/*.cpp
BIN=./bin/mc-raytracer

mc-raytracer:
	$(CXX) $(CPPFLAGS) $(SRC) -o $(BIN)