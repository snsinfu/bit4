module mechanics
    ! Dunno if this is interoperable with C...
    type system
        integer          :: particle_count
        double precision :: spring_constant
    end type
end module

subroutine init_positions(sys, pos)
    use mechanics
    implicit none

    type(system) :: sys
    double precision :: pos(sys%particle_count, 3)

    call random_number(pos)
end subroutine

subroutine compute_force_and_energy(sys, pos, forces, energy)
    use mechanics
    implicit none

    type(system) :: sys
    double precision :: pos(sys%particle_count, 3)
    double precision :: forces(sys%particle_count, 3)
    double precision :: energy
    integer :: i
    integer :: j
    double precision :: r(3)

    forces(:, :) = 0
    energy = 0

    do i = 1, sys%particle_count
        do j = i + 1, sys%particle_count
            r(:) = pos(i, :) - pos(j, :)
            forces(i, :) = forces(i, :) + sys%spring_constant * r(:)
            forces(j, :) = forces(j, :) - sys%spring_constant * r(:)
            energy = energy + sys%spring_constant * dot_product(r, r) / 2
        end do
    end do
end subroutine
