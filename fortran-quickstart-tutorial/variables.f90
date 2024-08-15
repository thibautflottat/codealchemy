program variables
    implicit none
    
    ! declare
    integer :: amount
    real :: pi
    complex :: freq
    character :: initial
    logical :: isOkay

    !init - don't init when declare
    amount = 10
    pi = 3.1415927
    freq = (1.0, -0.5)
    initial = 'A'
    isOkay = .false.

    print *, 'The value of amount (integer) is: ', amount
    print *, 'The value of pi (real) is: ', pi
    print *, 'The value of frequency (complex) is: ', freq
    print *, 'The value of initial (character) is: ', initial
    print *, 'The value of isOkay (logical) is: ', isOkay
end program variables