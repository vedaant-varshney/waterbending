#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <vector>
#include <GL/glu.h>
#include <cmath>
#include <iostream>
#include "test.hpp"

int NUM_CUBES_X = 16;
int NUM_CUBES_Y = 8;
int NUM_CUBES_Z = 8;

// TODO: Replace references to spheres with cubes
std::vector<Cube> generateGrid(int numSpheresX, int numSpheresY, int numSpheresZ, float spacing) {
    std::vector<Cube> cubes;
    
    // Calculate grid size based on the spacing and number of spheres
    float gridSizeX = (numSpheresX - 1) * spacing;
    float gridSizeY = (numSpheresY - 1) * spacing;
    float gridSizeZ = (numSpheresZ - 1) * spacing;

    for (int x = 0; x < numSpheresX; x++) {
        for (int y = 0; y < numSpheresY; y++) {
            for (int z = 0; z < numSpheresZ; z++) {
                // Position each sphere based on the spacing
                cubes.push_back({
                    x * spacing - gridSizeX / 2.0f, 
                    y * spacing - gridSizeY / 2.0f, 
                    z * spacing - gridSizeZ / 2.0f, 
                    0.0f,
                    0.0f,
                    0.0f
                });
            }
        }
    }
    return cubes;
}


void drawCube(float x, float y, float z, float r, float g, float b) {
    // TODO: Move alpha to drawCube parameters
    float alpha = 0.3f;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE); // Disable depth writing for proper blending

    glPushMatrix();
    glTranslatef(x, y, z);
    glColor4f(r, g, b, alpha);  // Use RGBA for transparency

    glBegin(GL_QUADS);
    
    // Front face
    glVertex3f(-0.5f, -0.5f,  0.5f);
    glVertex3f( 0.5f, -0.5f,  0.5f);
    glVertex3f( 0.5f,  0.5f,  0.5f);
    glVertex3f(-0.5f,  0.5f,  0.5f);
    
    // Back face
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f,  0.5f, -0.5f);
    glVertex3f( 0.5f,  0.5f, -0.5f);
    glVertex3f( 0.5f, -0.5f, -0.5f);
    
    // Left face
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f,  0.5f);
    glVertex3f(-0.5f,  0.5f,  0.5f);
    glVertex3f(-0.5f,  0.5f, -0.5f);
    
    // Right face
    glVertex3f( 0.5f, -0.5f, -0.5f);
    glVertex3f( 0.5f,  0.5f, -0.5f);
    glVertex3f( 0.5f,  0.5f,  0.5f);
    glVertex3f( 0.5f, -0.5f,  0.5f);
    
    // Top face
    glVertex3f(-0.5f,  0.5f, -0.5f);
    glVertex3f(-0.5f,  0.5f,  0.5f);
    glVertex3f( 0.5f,  0.5f,  0.5f);
    glVertex3f( 0.5f,  0.5f, -0.5f);
    
    // Bottom face
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f( 0.5f, -0.5f, -0.5f);
    glVertex3f( 0.5f, -0.5f,  0.5f);
    glVertex3f(-0.5f, -0.5f,  0.5f);

    glEnd();

    glPopMatrix();

    glDepthMask(GL_TRUE); // Re-enable depth writing
    glDisable(GL_BLEND);  // Disable blending after drawing
}

void drawOnGrid(const std::vector<Cube> & cubes, int x, int y, int z, float r, float g, float b) {
    Cube cube = cubes[x * (NUM_CUBES_Y*NUM_CUBES_Z) + y * NUM_CUBES_Z + z];
    drawCube(cube.x, cube.y, cube.z, r, g, b);

};


Cube getcube(const std::vector<Cube> & cubes, int x, int y, int z) {
    return cubes[x * (NUM_CUBES_Y*NUM_CUBES_Z) + y * NUM_CUBES_Z + z];
};


// Basic 3D sine wave simulation
float threeDWave(float amp, float k, float omega, float x, float y, float t) {
   return amp*sin(k*x - omega*t) + amp*sin(k*y-omega*t); 
}



int main() {
    // Initialize SDL and check for errors
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL Initialization Error: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("3D Grid", 100, 100, 800, 600, SDL_WINDOW_OPENGL);
    if (!window) {
        printf("SDL Window Creation Error: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        printf("SDL OpenGL Context Creation Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Initialize OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    gluPerspective(45.0, 800.0 / 600.0, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);

    float spacing = 1.7f;  // Set the desired spacing between cubes

    // Generate the grid of cubes with the given spacing
    std::vector<Cube> cubes = generateGrid(NUM_CUBES_X, NUM_CUBES_Y, NUM_CUBES_Z, spacing);

    bool running = true;
    SDL_Event event;
    float angle = 0.0f;
    float time = 0.0f;


    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        gluLookAt(0, 5, 40, 0, 0, 0, 0, 1, 0);
        
        glRotatef(angle, 0, 1, 0);

        
        for (int x = 0; x < NUM_CUBES_X; x++) {
            for (int y = 0; y < NUM_CUBES_Y; y++) {
                for (int z = 0; z < NUM_CUBES_Z; z++) {
                    
                    float yToRender = threeDWave(2, 0.25f, 1.2f, x, z, time);
                    yToRender += NUM_CUBES_Y/2 + 0.01;
                    int yIndToRender = floor(yToRender);
                    yIndToRender = std::max(yIndToRender, 0);

                    
                    if (y == yIndToRender) {
                        Cube cube = getcube(cubes, x, y, z);
                        // 28, 181, 237

                        drawCube(cube.x, cube.y, cube.z, 28.0f/255.0f, 181.0f/255.0f, 237.0f/255.0f);

                    }



                }
            }
        }

        time += 0.01666666666f;

        SDL_GL_SwapWindow(window);
        SDL_Delay(16); // ~60 FPS
        angle += 0.001f;
    }

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
