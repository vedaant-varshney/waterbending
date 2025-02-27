import pygame
from pygame.locals import *
from OpenGL.GL import *
from OpenGL.GLU import *
import numpy as np

# Constants
NUM_CUBES_X = 16
NUM_CUBES_Y = 8
NUM_CUBES_Z = 8
SPACING = 1.7

# Generates grid and gives x, y, z values for each element of grid of given dims with spacing
def generate_grid(num_x, num_y, num_z, spacing):
    grid = []
    grid_size_x = (num_x - 1) * spacing
    grid_size_y = (num_y - 1) * spacing
    grid_size_z = (num_z - 1) * spacing
    
    for x in range(num_x):
        for y in range(num_y):
            for z in range(num_z):
                grid.append((
                    x * spacing - grid_size_x / 2.0,
                    y * spacing - grid_size_y / 2.0,
                    z * spacing - grid_size_z / 2.0
                ))
    return np.reshape(grid, (num_x, num_y, num_z, 3))

def draw_cube(x, y, z, r, g, b, alpha=0.3):
    glEnable(GL_BLEND)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
    glDepthMask(GL_FALSE)
    
    glPushMatrix()
    glTranslatef(x, y, z)
    glColor4f(r, g, b, alpha)
    
    vertices = [
        (-0.5, -0.5,  0.5), (0.5, -0.5,  0.5), (0.5,  0.5,  0.5), (-0.5,  0.5,  0.5),  # Front
        (-0.5, -0.5, -0.5), (-0.5,  0.5, -0.5), (0.5,  0.5, -0.5), (0.5, -0.5, -0.5)   # Back
    ]
    
    faces = [
        (0, 1, 2, 3), (4, 5, 6, 7), (0, 3, 5, 4), 
        (1, 7, 6, 2), (3, 2, 6, 5), (0, 4, 7, 1)
    ]
    
    glBegin(GL_QUADS)
    for face in faces:
        for vertex in face:
            glVertex3fv(vertices[vertex])
    glEnd()
    
    glPopMatrix()
    glDepthMask(GL_TRUE)
    glDisable(GL_BLEND)

def three_d_wave(amp, k, omega, x, y, t):
    return amp * np.sin(k * x - omega * t) + amp * np.sin(k * y - omega * t)

def main():
    pygame.init()
    screen = pygame.display.set_mode((800, 600), DOUBLEBUF | OPENGL)
    
    glEnable(GL_DEPTH_TEST)
    glMatrixMode(GL_PROJECTION)
    gluPerspective(45, 800 / 600, 0.1, 100.0)
    glMatrixMode(GL_MODELVIEW)
    
    cubes = generate_grid(NUM_CUBES_X, NUM_CUBES_Y, NUM_CUBES_Z, SPACING)
    print(cubes.shape)
    
    running = True
    angle = 0.0
    time = 0.0

    
    while running:
        for event in pygame.event.get():
            if event.type == QUIT:
                running = False
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        glLoadIdentity()
        gluLookAt(0, 5, 40, 0, 0, 0, 0, 1, 0)
        glRotatef(angle, 0, 1, 0)
        
        for x in range(NUM_CUBES_X):
            for y in range(NUM_CUBES_Y):
                for z in range(NUM_CUBES_Z):
                    y_render = three_d_wave(2, 0.25, 1.2, x, z, time) + NUM_CUBES_Y / 2 + 0.01
                    y_index = max(int(y_render), 0)
                    if y == y_index:
                        cx, cy, cz = cubes[x, y, z]
                        draw_cube(cx, cy, cz, 28/255, 181/255, 237/255)
        
        time += 0.016
        pygame.display.flip()
        pygame.time.wait(16)
        angle += 0.01
    
    pygame.quit()

if __name__ == "__main__":
    main()
