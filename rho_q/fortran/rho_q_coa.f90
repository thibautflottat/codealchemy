module rho_q_module
  use iso_fortran_env, only: dp => real64
  implicit none

contains

  pure function dot(a, b) result(res)
    real(dp), intent(in) :: a(3), b(3)
    real(dp) :: res
    res = a(1)*b(1) + a(2)*b(2) + a(3)*b(3)
  end function dot

  subroutine rho_q(x, q, rho)
    real(dp), intent(in) :: x(:,:)[*], q(:,:)[*]
    complex(dp), intent(out) :: rho(:,:)[*]
    integer :: i, j, Nx, Nq, num_images, image_id
    real(dp) :: alpha

    Nx = size(x, 1)
    Nq = size(q, 1)
    num_images = num_images()
    image_id = this_image()

    !$omp parallel do private(j, alpha) schedule(guided)
    do i = image_id, Nq, num_images
      do j = 1, Nx
        alpha = dot(x(j,:), q(i,:))
        rho(i,j) = cmplx(cos(alpha), sin(alpha), kind=dp)
      end do
    end do
    !$omp end parallel do

    call co_sum(rho)
  end subroutine rho_q

end module rho_q_module

program main
  use rho_q_module
  use iso_fortran_env, only: dp => real64
  implicit none

  integer, parameter :: Nx = 10000, Nq = 10000
  real(dp), allocatable :: x(:,:)[:], q(:,:)[:]
  complex(dp), allocatable :: rho(:,:)[:]
  real(dp) :: start_time, end_time
  integer :: i, j

  allocate(x(Nx,3)[*], q(Nq,3)[*], rho(Nq,Nx)[*])

  ! Initialize random number generator
  call random_seed()

  ! Generate random data
  call random_number(x)
  call random_number(q)

  ! Time the execution
  call cpu_time(start_time)
  call rho_q(x, q, rho)
  call cpu_time(end_time)

  if (this_image() == 1) then
    print '("Fortran Execution Time: ", F0.6, " seconds")', end_time - start_time
  end if

  deallocate(x, q, rho)

end program main