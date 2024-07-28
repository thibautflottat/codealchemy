module pairwise
    implicit none
    integer, parameter :: sp = selected_real_kind(6, 37)
    integer, parameter :: dp = selected_real_kind(15, 307)
    
contains

    subroutine pairwise_distances(positions, n_particles, distances)
        implicit none

        integer, intent(in) :: n_particles
        real(sp), intent(in) :: positions(n_particles, 3)
        real(sp), intent(out) :: distances(n_particles, n_particles)
        integer :: i, j
        integer :: start_clock, end_clock, clock_rate
        real(sp) :: L, half_L, dx, dy, dz, dist, exec_time

        L = 1.0_sp
        half_L = 0.5_sp
        distances = 0.0_sp

        ! Get the clock rate
        call system_clock(count_rate=clock_rate)
        ! Get the start time
        call system_clock(start_clock)

        ! do k = 1, n_particles
        !     print *, 'positions(', k, ',:) = ', positions(k, :)
        ! end do

        !$omp parallel private(i, j, dx, dy, dz, dist)
        !$omp do
        do i = 1, n_particles
            do j = i + 1, n_particles
                dx = positions(i, 1) - positions(j, 1)
                dy = positions(i, 2) - positions(j, 2)
                dz = positions(i, 3) - positions(j, 3)
    
                ! Apply periodic boundary conditions
                if (dx > half_L) dx = dx - L
                if (dx < -half_L) dx = dx + L
                if (dy > half_L) dy = dy - L
                if (dy < -half_L) dy = dy + L
                if (dz > half_L) dz = dz - L
                if (dz < -half_L) dz = dz + L
    
                dist = sqrt(dx**2 + dy**2 + dz**2)
                distances(i, j) = dist
                distances(j, i) = dist
            end do
        end do
        !$omp end do
        !$omp end parallel

        ! Get the end time
        call system_clock(end_clock)
  
        ! Calculate the execution time
        exec_time = real(end_clock - start_clock) / real(clock_rate)
        print *, 'Execution time (seconds):', exec_time

        ! Debugging: Print the distances matrix
        do i = 1, n_particles
            do j = 1, n_particles
                print *, 'distances(', i, ',', j, ') = ', distances(i, j)
            end do
        end do

    end subroutine pairwise_distances

end module pairwise