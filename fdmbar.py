import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.animation import FuncAnimation

# Parameters for the simulation
Lx = 10.0  # Length of the domain in x-direction
Ly = 10.0  # Length of the domain in y-direction
Nx = 10    # Number of grid points in x-direction
Ny = 10    # Number of grid points in y-direction
Tmax = 4.0  # Maximum time
c = 2.0     # Wave speed

# Discretization
dx = Lx / (Nx - 1)
dy = Ly / (Ny - 1)
dt = 0.05   # Time step
Nt = int(Tmax / dt)  # Number of time steps

# Create meshgrid for space
x = np.linspace(0, Lx, Nx)
y = np.linspace(0, Ly, Ny)
X, Y = np.meshgrid(x, y)

# Initial conditions
u = np.zeros((Nx, Ny))  # Initial displacement
u[Nx//2, Ny//2] = 0.1
u_new = np.zeros((Nx, Ny))  # New displacement (next time step)
u_old = np.zeros((Nx, Ny))  # Old displacement (previous time step)

# Function to update the wave field using the finite difference method
def update_wave(u, u_old, u_new, dt, dx, dy, c):
    # Calculate the new displacement based on FDM
    u_new[1:-1, 1:-1] = (2 * u[1:-1, 1:-1] - u_old[1:-1, 1:-1] +
                         c**2 * dt**2 * (
                             (u[2:, 1:-1] - 2 * u[1:-1, 1:-1] + u[:-2, 1:-1]) / dx**2 +
                             (u[1:-1, 2:] - 2 * u[1:-1, 1:-1] + u[1:-1, :-2]) / dy**2
                         ))
    return u_new

# Set up the figure and 3D axis for plotting
fig = plt.figure(figsize=(10, 8))
ax = fig.add_subplot(111, projection='3d')
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Displacement')
ax.set_title(f'Time: 0.00 s')

# Generate grid for bar chart positioning
xpos, ypos = np.meshgrid(x, y)
xpos = xpos.flatten()
ypos = ypos.flatten()
zpos = np.zeros_like(xpos)

# Bar width and depth
dx = dy = 1.0  # Setting the bar size to 1x1 in the x and y directions


def add_jump(u, time, t_jump, center_x, center_y, amplitude=0.1, width=1.0):
    """ Add a Gaussian-like pulse disturbance to the displacement at time t_jump. """
    if time == t_jump:
        # Applying a Gaussian pulse at (center_x, center_y)
        dist = np.sqrt((X - center_x)**2 + (Y - center_y)**2)
        u += amplitude * np.exp(-dist**2 / (2 * width**2))  # Gaussian shape
    return u

# Function to update the plot for each frame
def animate(t):
    global u, u_old, u_new
    # Update the wave field

    u = add_jump(u, t, t_jump=2.0, center_x=0, center_y=0)  # First jump at t=2.0


    u_new = update_wave(u, u_old, u_new, dt, dx, dy, c)
    

    # Clear the previous frame
    ax.cla()
    
    # Plot the bars for the current wave field
    ax.bar3d(xpos, ypos, zpos, dx, dy, u_new.flatten(), cmap='viridis')

    # Set the plot labels again
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Displacement')
    ax.set_title(f'Time: {t * dt:.2f} s')

    ax.set_xlim(0, Lx)
    ax.set_ylim(0, Ly)
    ax.set_zlim(-1, 1)

    # Update the old and current displacement for the next time step
    u_old = u.copy()
    u = u_new.copy()

    return [ax]

# Create the animation
ani = FuncAnimation(fig, animate, frames=Nt, interval=50, blit=False)
ani.save("testing2.gif", writer="ffmpeg")
