import numpy as np
import matplotlib.pyplot as plt
from scipy.fft import fft2, ifft2

# Parameters
Lx, Ly = 64, 64  # System size
dx, dy = 1.0, 1.0  # Grid spacing
Nx, Ny = int(Lx/dx), int(Ly/dy)  # Number of grid points
NA = 100  # Number of segments in polymer A
NB = 100  # Number of segments in polymer B (same as NA)
ratio_A = 0.5  # Fraction of polymer A in the mixture
ratio_B = 0.5  # Fraction of polymer B in the mixture
chi = 2.0  # Moderate Flory-Huggins parameter
max_iter = 1000  # Maximum number of SCFT iterations
tolerance = 1e-6  # Convergence criterion
mixing_parameter = 0.1  # Under-relaxation parameter

# Boundary condition type: 'periodic', 'neumann', 'dirichlet'
boundary_condition_type = 'periodic'

# Initialize fields with random noise to break symmetry
np.random.seed(42)
wA = 0.1 * np.random.normal(0, 1, (Nx, Ny))
wB = 0.1 * np.random.normal(0, 1, (Nx, Ny))

# Define boundary condition function
def apply_boundary_conditions(field):
    if boundary_condition_type == 'periodic':
        # Periodic boundary conditions
        field = np.pad(field, ((1, 1), (1, 1)), mode='wrap')
        field = field[1:-1, 1:-1]
    elif boundary_condition_type == 'neumann':
        # Neumann boundary conditions (zero-gradient)
        field = np.pad(field, ((1, 1), (1, 1)), mode='edge')
        field = field[1:-1, 1:-1]
    elif boundary_condition_type == 'dirichlet':
        # Dirichlet boundary conditions (fixed value at boundary)
        field = np.pad(field, ((1, 1), (1, 1)), mode='constant', constant_values=0)
        field = field[1:-1, 1:-1]
    return field

# Discrete Laplacian in Fourier space
kx = 2 * np.pi * np.fft.fftfreq(Nx, d=dx)
ky = 2 * np.pi * np.fft.fftfreq(Ny, d=dy)
kx, ky = np.meshgrid(kx, ky)
Laplacian = -(kx**2 + ky**2)

# SCFT iterations with boundary conditions
for iteration in range(max_iter):
    # Apply boundary conditions to fields
    wA = apply_boundary_conditions(wA)
    wB = apply_boundary_conditions(wB)
    
    # Solve for polymer A propagators qA and qA_dagger
    qA = np.ones((Nx, Ny, NA+1))
    qA_dagger = np.ones((Nx, Ny, NA+1))

    for s in range(1, NA+1):
        qA[:, :, s] = qA[:, :, s-1] * np.exp(-dx * wA)
    for s in range(NA-1, -1, -1):
        qA_dagger[:, :, s] = qA_dagger[:, :, s+1] * np.exp(-dx * wA)

    # Solve for polymer B propagators qB and qB_dagger
    qB = np.ones((Nx, Ny, NB+1))
    qB_dagger = np.ones((Nx, Ny, NB+1))

    for s in range(1, NB+1):
        qB[:, :, s] = qB[:, :, s-1] * np.exp(-dx * wB)
    for s in range(NB-1, -1, -1):
        qB_dagger[:, :, s] = qB_dagger[:, :, s+1] * np.exp(-dx * wB)

    # Compute densities for A and B
    qA_integral = np.sum(qA[:, :, :NA] * qA_dagger[:, :, :NA], axis=2)
    qB_integral = np.sum(qB[:, :, :NB] * qB_dagger[:, :, :NB], axis=2)

    phiA = ratio_A * qA_integral / (NA * (qA_integral + qB_integral))
    phiB = ratio_B * qB_integral / (NB * (qA_integral + qB_integral))

    # Normalize the volume fractions so that phi_A + phi_B = 1 at each grid point
    total_phi = phiA + phiB
    phiA_normalized = phiA / total_phi
    phiB_normalized = phiB / total_phi

    # Clamp values to ensure they are within the [0, 1] range
    phiA_normalized = np.clip(phiA_normalized, 0.01, 0.99)
    phiB_normalized = np.clip(phiB_normalized, 0.01, 0.99)

    # Update fields with under-relaxation
    wA_new = chi * phiB_normalized
    wB_new = chi * phiA_normalized

    wA_new = np.real(ifft2(fft2(wA_new) / (1 - Laplacian)))
    wB_new = np.real(ifft2(fft2(wB_new) / (1 - Laplacian)))

    wA = (1 - mixing_parameter) * wA + mixing_parameter * wA_new
    wB = (1 - mixing_parameter) * wB + mixing_parameter * wB_new

    # Check for convergence
    delta_wA = np.linalg.norm(wA_new - wA) / np.linalg.norm(wA)
    delta_wB = np.linalg.norm(wB_new - wB) / np.linalg.norm(wB)

    if delta_wA < tolerance and delta_wB < tolerance:
        print(f'Converged after {iteration} iterations.')
        break
else:
    print('SCFT did not converge within the maximum number of iterations.')

# Print mean values
print("phiA_normalized.mean() =", phiA_normalized.mean())
print("phiB_normalized.mean() =", phiB_normalized.mean())

# Plot volume fraction distributions
plt.figure(figsize=(12, 5))
plt.subplot(1, 2, 1)
plt.imshow(phiA_normalized, cmap='viridis', vmin=0, vmax=1)
plt.colorbar()
plt.title('Volume Fraction of Polymer A')

plt.subplot(1, 2, 2)
plt.imshow(phiB_normalized, cmap='viridis', vmin=0, vmax=1)
plt.colorbar()
plt.title('Volume Fraction of Polymer B')

plt.show()