program array
    implicit none
    
    integer, dimension(:), allocatable :: array1
    integer, dimension(10) :: array2
    real, dimension(3, 3) :: array3
    ! Custom lower and upper index bounds
    real :: array4(0:9)
    real :: array5(-5:5)
    print *, array4
    print *, array5
end program array