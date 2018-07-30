program path_counting
    implicit none
    integer count_paths

    print *, count_paths(3, 1, 1, 1)
end program

function count_paths(n, xstart, ystart, zstart)
    integer count_paths
    integer n
    integer xstart
    integer ystart
    integer zstart
    integer stack(0:n*n*n, 0:3)
    integer occupied(n, n, n)
    integer depth
    integer total
    integer stage
    integer x
    integer y
    integer z

    stack(0, 0) = 0
    stack(0, 1) = xstart
    stack(0, 2) = ystart
    stack(0, 3) = zstart

    occupied(:, :, :) = 0

    depth = 0
    total = 0

    do
        if (depth < 0) then
            exit
        end if

        stage = stack(depth, 0)
        x = stack(depth, 1)
        y = stack(depth, 2)
        z = stack(depth, 3)

        if (stage == 0) then

            if (x < 1 .or. y < 1 .or. z < 1 .or. x > n .or. y > n .or. z > n .or. occupied(x, y, z) == 1) then
                depth = depth - 1
                cycle
            end if

            if (depth + 1 == n * n * n) then
                total = total + 1
                depth = depth - 1
                cycle
            end if

            occupied(x, y, z) = 1

            stack(depth + 1, 0) = 0
            stack(depth + 1, 1) = x + 1
            stack(depth + 1, 2) = y
            stack(depth + 1, 3) = z
            stack(depth, 0) = stage + 1
            depth = depth + 1

        else if (stage == 1) then

            stack(depth + 1, 0) = 0
            stack(depth + 1, 1) = x - 1
            stack(depth + 1, 2) = y
            stack(depth + 1, 3) = z
            stack(depth, 0) = stage + 1
            depth = depth + 1

        else if (stage == 2) then

            stack(depth + 1, 0) = 0
            stack(depth + 1, 1) = x
            stack(depth + 1, 2) = y + 1
            stack(depth + 1, 3) = z
            stack(depth, 0) = stage + 1
            depth = depth + 1

        else if (stage == 3) then

            stack(depth + 1, 0) = 0
            stack(depth + 1, 1) = x
            stack(depth + 1, 2) = y - 1
            stack(depth + 1, 3) = z
            stack(depth, 0) = stage + 1
            depth = depth + 1

        else if (stage == 4) then

            stack(depth + 1, 0) = 0
            stack(depth + 1, 1) = x
            stack(depth + 1, 2) = y
            stack(depth + 1, 3) = z + 1
            stack(depth, 0) = stage + 1
            depth = depth + 1

        else if (stage == 5) then

            stack(depth + 1, 0) = 0
            stack(depth + 1, 1) = x
            stack(depth + 1, 2) = y
            stack(depth + 1, 3) = z - 1
            stack(depth, 0) = stage + 1
            depth = depth + 1

        else if (stage == 6) then

            occupied(x, y, z) = 0
            depth = depth - 1

        end if
    end do

    count_paths = total
end function
