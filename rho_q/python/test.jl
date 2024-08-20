using LinearAlgebra
using Random
using Base.Threads

function compute_rho(Nx::Int, Nq::Int)
    # Generate random data
    x = rand(Nx, 3)
    q = rand(Nq, 3)
    rho = zeros(ComplexF64, Nq)

    # Compute dot product and rho in one go
    @threads for j in 1:Nq
        alpha = dot(x[:, 1], q[j, 1]) + dot(x[:, 2], q[j, 2]) + dot(x[:, 3], q[j, 3])
        rho[j] = sum(exp.(1im * alpha))
    end

    return rho
end

# Time the execution of the optimized Julia code
Nx = 100000
Nq = 100000

start_time = time()
rho = compute_rho(Nx, Nq)
end_time = time()

println("Optimized Julia Execution Time: $(end_time - start_time) seconds")