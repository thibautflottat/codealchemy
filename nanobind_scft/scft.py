import numpy as np
from scipy.ndimage import gaussian_filter
import pyvista as pv

# Parameters
Nx, Ny, Nz = 64, 64, 64  # Grid size
dx = 0.1  # Grid spacing
f_H = 0.8  # Volume fraction of the homopolymer
f_P = 1.0 - f_H  # Volume fraction of the plasticizer
chi_HP = 15.0  # High interaction parameter between homopolymer and plasticizer
tolerance = 1e-5  # Convergence tolerance
max_iter = 500  # Maximum number of iterations
N_steps = 100  # Number of chain steps in propagator calculation
sigma = 1.0  # Standard deviation for Gaussian filter, simulating diffusion

# Initialize fields with small random variations to break symmetry
w_H = np.random.normal(loc=0.0, scale=0.1, size=(Nx, Ny, Nz))
w_P = np.random.normal(loc=0.0, scale=0.1, size=(Nx, Ny, Nz))

# Function to compute the chain propagators
def compute_propagator(w, N_steps):
    q = np.ones((Nx, Ny, Nz))  # Initial condition: uniform distribution
    for step in range(N_steps):
        q *= np.exp(-w * dx / N_steps)
        q = gaussian_filter(q, sigma=sigma)
    return q

# Main SCFT loop
for iteration in range(max_iter):
    # Compute the propagators for the homopolymer and plasticizer
    q_H = compute_propagator(w_H, N_steps)
    q_P = compute_propagator(w_P, N_steps)

    # Calculate volume fractions
    phi_H = q_H / (q_H + q_P)
    phi_P = q_P / (q_H + q_P)

    # Enforce the conservation of the total volume fractions
    phi_H *= f_H / np.mean(phi_H)
    phi_P *= f_P / np.mean(phi_P)

    # Update the fields based on the volume fractions
    w_H_new = chi_HP * phi_P
    w_P_new = chi_HP * phi_H

    # Check for convergence
    delta_w_H = np.linalg.norm(w_H_new - w_H)
    delta_w_P = np.linalg.norm(w_P_new - w_P)
    if max(delta_w_H, delta_w_P) < tolerance:
        print(f"Converged after {iteration + 1} iterations.")
        break

    # Update fields for the next iteration
    w_H = w_H_new
    w_P = w_P_new
else:
    print("Failed to converge.")

# Output the final volume fraction ranges
print(f"phi_H range: {phi_H.min()} to {phi_H.max()}")
print(f"phi_P range: {phi_P.min()} to {phi_P.max()}")

# Visualization using PyVista for 3D rendering
grid = pv.ImageData(
    dimensions=(Nx, Ny, Nz),
    spacing=(dx, dx, dx),
    origin=(0, 0, 0),
)

# Add volume fraction data to the grid
grid["phi_H"] = phi_H.flatten(order="F")
grid["phi_P"] = phi_P.flatten(order="F")

# Choose contour levels based on the phase separation
contour_levels_H = np.linspace(0.7, 1.0, 3)  # High polymer concentration
contour_levels_P = np.linspace(0.0, 0.3, 3)  # Low polymer concentration (high plasticizer)

# Visualization
plotter = pv.Plotter()

# Create contours for homopolymer and plasticizer
contour_H = grid.contour(contour_levels_H, scalars="phi_H")
contour_P = grid.contour(contour_levels_P, scalars="phi_P")

# Add the contours to the plotter with different colors
plotter.add_mesh(contour_H, color="blue", opacity=0.5, label="Homopolymer")
plotter.add_mesh(contour_P, color="green", opacity=0.5, label="Plasticizer")

plotter.show()