CXX=g++
CPPFLAGS=-Xpreprocessor -fopenmp -stdlib=libc++ -std=c++17 -O3 -mtune=intel -mavx2 -DNDEBUG -lomp 
SRC=./src/*.cpp
BIN=./bin/mc-raytracer


mc-raytracer:
	$(CXX) $(CPPFLAGS) $(SRC) -o $(BIN)