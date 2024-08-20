using LinearAlgebra
using Random
using Base.Threads

function compute_rho(Nx::Int, Nq::Int)
    # Generate random data
    x = rand(Nx, 3)
    q = rand(Nq, 3)
    rho = zeros(ComplexF64, Nq)

    # Parallel computation of rho
    @threads for j in 1:Nq
        sum_rho = 0.0 + 0.0im
        @inbounds for i in 1:Nx
            alpha = dot(x[i, :], q[j, :])
            sum_rho += exp(1im * alpha)
        end
        rho[j] = sum_rho
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