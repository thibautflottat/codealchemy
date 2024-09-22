import itertools
import numpy as np
from numpy.typing import NDArray
# from dynasor.logging_tools import logger

def get_spherical_qpoints(
        cell: NDArray[float],
        q_max: float,
):

    # inv(A.T) == inv(A).T
    # The physicists reciprocal cell
    rec_cell = np.linalg.inv(cell.T) * 2 * np.pi


    # We want to find all points on the lattice defined by the reciprocal cell
    # such that all points within max_q are in this set
    inv_rec_cell = np.linalg.inv(rec_cell.T)  # cell / 2pi

    # h is the height of the rec_cell perpendicular to the other two vectors
    h = 1 / np.linalg.norm(inv_rec_cell, axis=1)

    # If a q_point has a coordinate larger than this number it must be further away than q_max
    N = np.ceil(q_max / h).astype(int)

    # Create q-points in the first octant
    lattice_points = list(itertools.product(*[range(n+1) for n in N]))

    q_points = lattice_points @ rec_cell

    # Calculate distances for pruning
    q_distances = np.linalg.norm(q_points, axis=1)  # Find distances


    # Sort distances and q-points based on distance
    argsort = np.argsort(q_distances)
    q_distances = q_distances[argsort]
    q_points = q_points[argsort]

    # Prune based on distances
    q_points = q_points[q_distances <= q_max]
    q_distances = q_distances[q_distances <= q_max]

    print(q_points)
    return None
    # return q_points

cell=np.array([[10,0,0],[0,10,0],[0,0,10]])
q_points = get_spherical_qpoints(cell=cell, q_max=2)
print(q_points)