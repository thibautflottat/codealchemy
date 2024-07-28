use std::env;
use std::sync::{Arc, Mutex};
use std::time::Instant;
use rand::Rng;
use rayon::prelude::*;

#[derive(Debug)]
struct Pairwise {
    sp: f32,
    dp: f64,
}

impl Pairwise {
    fn new() -> Self {
        Pairwise {
            sp: 1.0e-6_f32,
            dp: 1.0e-15_f64,
        }
    }

    fn pairwise_distances(&self, positions: &Vec<[f32; 3]>, n_particles: usize) -> Vec<Vec<f32>> {
        let l = 1.0_f32;
        let half_l = 0.5_f32;

        let distances = Arc::new(Mutex::new(vec![vec![0.0_f32; n_particles]; n_particles]));
        let start_time = Instant::now();

        (0..n_particles).into_par_iter().for_each(|i| {
            for j in (i + 1)..n_particles {
                let mut dx = positions[i][0] - positions[j][0];
                let mut dy = positions[i][1] - positions[j][1];
                let mut dz = positions[i][2] - positions[j][2];

                // Apply periodic boundary conditions
                if dx > half_l {
                    dx -= l;
                }
                if dx < -half_l {
                    dx += l;
                }
                if dy > half_l {
                    dy -= l;
                }
                if dy < -half_l {
                    dy += l;
                }
                if dz > half_l {
                    dz -= l;
                }
                if dz < -half_l {
                    dz += l;
                }

                let dist = (dx * dx + dy * dy + dz * dz).sqrt();
                let mut distances = distances.lock().unwrap();
                distances[i][j] = dist;
                distances[j][i] = dist;
            }
        });

        let duration = start_time.elapsed();
        println!("Execution time (seconds): {:.6}", duration.as_secs_f64());

        let distances = Arc::try_unwrap(distances)
            .expect("Failed to unwrap Arc")
            .into_inner()
            .expect("Failed to get inner mutex value");
        
        distances
    }
}

fn main() {
    // Get the number of particles from the command line
    let args: Vec<String> = env::args().collect();
    let n_particles: usize = args.get(1)
        .expect("Please provide the number of particles")
        .parse()
        .expect("Invalid number of particles");

    // Allocate arrays
    let mut positions: Vec<[f32; 3]> = vec![[0.0; 3]; n_particles];

    // Initialize positions (example: random initialization)
    let mut rng = rand::thread_rng();
    for i in 0..n_particles {
        positions[i] = [rng.gen(), rng.gen(), rng.gen()];
    }

    // Call the subroutine to compute pairwise distances
    let pairwise = Pairwise::new();
    let distances = pairwise.pairwise_distances(&positions, n_particles);

    // Use the distances to avoid the unused variable warning
    // Here we print the first few distances as an example
    // for i in 0..n_particles.min(5) {
    //     for j in 0..n_particles.min(5) {
    //         println!("distances({}, {}) = {}", i, j, distances[i][j]);
    //     }
    // }
}

