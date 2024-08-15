program arithmetic
    implicit none
    
    real :: pi
    real :: radius
    real :: height
    real :: area
    real :: volume

    pi = 3.14

    print *, 'Enter radius'
    read(*,*) radius

    area = pi * radius ** 2
    print *, 'Area is:', area
end program arithmetic