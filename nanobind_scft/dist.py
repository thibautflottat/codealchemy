import numpy as np
from scipy.spatial import cKDTree

def sort_pairs(pairs):
    """
    Sort pairs of indices and their corresponding distances.
    
    Parameters:
        pairs (list): List of tuples (i, j, distance).
        
    Returns:
        sorted_pairs (list): Sorted list of tuples (i, j, distance) based on (i, j).
    """
    return sorted(pairs, key=lambda x: (min(x[0], x[1]), max(x[0], x[1])))


def create_cell_list(positions, box_size, cutoff):
    """
    Create a cell list for efficient neighbor search.
    
    Parameters:
        positions (np.ndarray): Array of positions of shape (N, 3).
        box_size (float): Size of the cubic simulation box.
        cutoff (float): Cutoff distance for neighbor search.
        
    Returns:
        cells (dict): Dictionary mapping cell indices to lists of particle indices.
        cell_indices (np.ndarray): Array of shape (N, 3) storing the cell index of each particle.
        cell_size (float): Size of each cell.
        num_cells (int): Number of cells along each dimension.
    """
    num_cells = int(box_size // cutoff)
    cell_size = box_size / num_cells

    # Compute cell indices for each particle
    cell_indices = np.floor(positions / cell_size).astype(int)

    # Handle periodic boundary conditions
    cell_indices = np.mod(cell_indices, num_cells)

    # Create the cell list
    cells = {}
    for idx, cell_index in enumerate(cell_indices):
        cell_index_tuple = tuple(cell_index)
        if cell_index_tuple not in cells:
            cells[cell_index_tuple] = []
        cells[cell_index_tuple].append(idx)

    return cells, cell_indices, cell_size, num_cells

def get_neighboring_cells(cell_index, num_cells):
    """
    Get the indices of all neighboring cells (including the cell itself).
    
    Parameters:
        cell_index (tuple): Index of the current cell.
        num_cells (int): Number of cells along each dimension.
        
    Returns:
        neighbors (list): List of neighboring cell indices.
    """
    neighbors = []
    for dx in [-1, 0, 1]:
        for dy in [-1, 0, 1]:
            for dz in [-1, 0, 1]:
                neighbor_index = (
                    (cell_index[0] + dx) % num_cells,
                    (cell_index[1] + dy) % num_cells,
                    (cell_index[2] + dz) % num_cells,
                )
                neighbors.append(neighbor_index)
    return neighbors

def compute_distances(positions, cells, cell_indices, cell_size, num_cells, box_size, cutoff):
    """
    Compute distances between particles within the given cutoff.
    
    Parameters:
        positions (np.ndarray): Array of positions of shape (N, 3).
        cells (dict): Cell list dictionary.
        cell_indices (np.ndarray): Array of cell indices for each particle.
        cell_size (float): Size of each cell.
        num_cells (int): Number of cells along each dimension.
        box_size (float): Size of the cubic simulation box.
        cutoff (float): Cutoff distance for neighbor search.
        
    Returns:
        pairs (list): List of tuples (i, j, distance) for all pairs within the cutoff.
    """
    cutoff_squared = cutoff ** 2
    pairs = []

    for cell_index, particles in cells.items():
        neighbors = get_neighboring_cells(cell_index, num_cells)

        for particle in particles:
            for neighbor_cell in neighbors:
                if neighbor_cell in cells:
                    for neighbor_particle in cells[neighbor_cell]:
                        if particle < neighbor_particle:  # Avoid double counting
                            delta = positions[particle] - positions[neighbor_particle]

                            # Apply minimum image convention for periodic boundary conditions
                            delta -= np.round(delta / box_size) * box_size

                            dist_squared = np.sum(delta ** 2)
                            if dist_squared < cutoff_squared:
                                pairs.append((particle, neighbor_particle, np.sqrt(dist_squared)))

    return sort_pairs(pairs)

def compute_distances_brute_force(positions, box_size, cutoff):
    cutoff_squared = cutoff ** 2
    pairs = []
    num_particles = len(positions)

    for i in range(num_particles):
        for j in range(i + 1, num_particles):
            delta = positions[i] - positions[j]
            delta -= np.round(delta / box_size) * box_size
            dist_squared = np.sum(delta ** 2)
            if dist_squared < cutoff_squared:
                pairs.append((i, j, np.sqrt(dist_squared)))

    return sort_pairs(pairs)  # Sort the pairs before returning

def compute_distances_kdtree_periodic(positions, box_size, cutoff):
    """
    Compute distances between particles using cKDTree with periodic boundary conditions.
    
    Parameters:
        positions (np.ndarray): Array of positions of shape (N, 3).
        box_size (float): Size of the cubic simulation box.
        cutoff (float): Cutoff distance for neighbor search.
        
    Returns:
        pairs (list): List of tuples (i, j, distance) for all pairs within the cutoff.
    """
    # Create cKDTree with periodic boundary conditions
    tree = cKDTree(positions, boxsize=box_size)
    
    # Query the tree for pairs within the cutoff and return distances
    pairs_indices, distances = tree.query(positions, k=2, distance_upper_bound=cutoff)
    # Combine the indices and distances
    pairs = [(i, j, dist) for (i, j), dist in zip(pairs_indices, distances)]
    return sort_pairs(pairs)  # Sort the pairs before returning



import time

# Example setup
np.random.seed(0)
num_particles = 100
box_size = 10.0
cutoff = 2.5
positions = np.random.rand(num_particles, 3) * box_size

# Brute Force Method
start_time = time.time()
pairs_brute_force = compute_distances_brute_force(positions, box_size, cutoff)
brute_force_time = time.time() - start_time
print(f"Brute Force Method: {len(pairs_brute_force)} pairs found in {brute_force_time:.4f} seconds")

# KD-Tree Method with Periodic Boundary Conditions
# start_time = time.time()
# pairs_kdtree_periodic = compute_distances_kdtree_periodic(positions, box_size, cutoff)
# kdtree_periodic_time = time.time() - start_time
# print(f"KD-Tree (Periodic) Method: {len(pairs_kdtree_periodic)} pairs found in {kdtree_periodic_time:.4f} seconds")

# Cell Lists Method
start_time = time.time()
cells, cell_indices, cell_size, num_cells = create_cell_list(positions, box_size, cutoff)
pairs_cell_list = compute_distances(positions, cells, cell_indices, cell_size, num_cells, box_size, cutoff)
cell_list_time = time.time() - start_time
print(f"Cell Lists Method: {len(pairs_cell_list)} pairs found in {cell_list_time:.4f} seconds")

# Compare Results
# print("\nComparison of Results:")
# for bf, kd, cl in zip(pairs_brute_force, pairs_kdtree_periodic, pairs_cell_list):
#     assert bf == kd == cl, "Mismatch found!"
#     print(f"Pair {bf[0]}, {bf[1]} -> Distance: {bf[2]:.4f}")
print("\nComparison of Results:")
for bf, cl in zip(pairs_brute_force, pairs_cell_list):
    assert bf == cl, "Mismatch found!"
    print(f"Pair {bf[0]}, {bf[1]} -> Distance: {bf[2]:.4f}")


https://stackoverflow.com/questions/21285058/find-all-point-pairs-closer-than-a-given-maximum-distance