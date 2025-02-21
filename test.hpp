#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <vector>
#include <GL/glu.h>
#include <cmath>

struct Cube {
    float x, y, z;
    float r, g, b;
};


std::vector<Cube> generateGrid(int numSpheresX, int numSpheresY, int numSpheresZ, float spacing);
// Draws cube at a specific position with given rgb values
void drawCube(float x, float y, float z, float r, float g, float b);


// Handles indexing as if cubes vector was matrix for drawing cube
void drawOnGrid(const std::vector<Cube> & cubes, int x, int y, int z, float r, float g, float b);

// Get cube element associated with matrix indices 
Cube getcube(const std::vector<Cube> & cubes, int x, int y, int z);
