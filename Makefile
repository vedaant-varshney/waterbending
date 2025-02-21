CXX = g++
CXXFLAGS = -Wall -std=c++17
LDFLAGS = -lSDL2 -lGL -lGLU 
TARGET = sdl_3d_grid
SRC = test.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)
