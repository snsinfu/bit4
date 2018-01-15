module mechanics
    implicit none

    type state
        double precision, allocatable :: positions(:, :)
        double precision, allocatable :: velocities(:, :)
    end type
end module

program main
    implicit none

    use mechanics

    type(state) :: st
    integer :: n

    n = 123

    allocate(st%positions(n, 3))
    allocate(st%velocities(n, 3))

    deallocate(st%velocities)
    deallocate(st%positions)
end program
