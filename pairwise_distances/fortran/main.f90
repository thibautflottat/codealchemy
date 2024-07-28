program main
    use pairwise
    implicit none

    integer :: n_particles
    integer :: seed
    character(len=10) :: arg

    real(sp), allocatable :: positions(:,:)
    real(sp), allocatable :: distances(:,:)

    ! Get the number of particles from the command line
    call get_command_argument(1, arg)
    read(arg, *) n_particles

    ! Allocate arrays
    allocate(positions(n_particles, 3))
    allocate(distances(n_particles, n_particles))

    ! Initialize positions (example: random initialization)
    call random_seed(seed)
    call random_number(positions)

    ! Call the subroutine to compute pairwise distances
    call pairwise_distances(positions, n_particles, distances)

    ! Deallocate memory
    deallocate(positions)
    deallocate(distances)

end program main