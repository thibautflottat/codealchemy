using LinearAlgebra
using Random
using Base.Threads

# Inline the dot product to reduce overhead
@inline function dot3(x1, x2, x3, y1, y2, y3)
    return x1 * y1 + x2 * y2 + x3 * y3
end

function compute_rho!(x::Matrix{Float64}, q::Matrix{Float64}, rho::Matrix{ComplexF64})
    Nx = size(x, 1)
    Nq = size(q, 1)

    # Parallel computation of rho with SIMD
    @threads for i in 1:Nq
        q1, q2, q3 = q[i, 1], q[i, 2], q[i, 3]
        @simd for j in 1:Nx
            x1, x2, x3 = x[j, 1], x[j, 2], x[j, 3]
            alpha = dot3(x1, x2, x3, q1, q2, q3)
            # Delaying complex computation as long as possible
            @inbounds rho[i, j] = exp(1im * alpha)
        end
    end
end

# Define the sizes
Nx = 20000
Nq = 20000

# Generate random data
x = rand(Nx, 3)
q = rand(Nq, 3)
rho = zeros(ComplexF64, Nq, Nx)

# Time the execution of the optimized Julia code
start_time = time()
compute_rho!(x, q, rho)
end_time = time()

println("Optimized Julia Execution Time: $(end_time - start_time) seconds")