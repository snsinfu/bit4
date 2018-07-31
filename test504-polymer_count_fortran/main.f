      program main
        implicit none
        integer count_paths

        write(*,*) count_paths(3, 1, 1, 1)

        stop
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

        occupied(:, :, :) = 0

        stack(0, :) = (/ 0, xstart, ystart, zstart /)

        depth = 0
        total = 0

        do
          if (depth .lt. 0) then
            exit
          end if

          stage = stack(depth, 0)
          x = stack(depth, 1)
          y = stack(depth, 2)
          z = stack(depth, 3)

          if (stage .eq. 0) then

            if (x .lt. 1 .or. y .lt. 1 .or. z .lt. 1 .or.
     &          x .gt. n .or. y .gt. n .or. z .gt. n .or.
     &          occupied(x, y, z) .eq. 1) then
              depth = depth - 1
              cycle
            end if

            if (depth + 1 .eq. n * n * n) then
              total = total + 1
              depth = depth - 1
              cycle
            end if

            occupied(x, y, z) = 1

            stack(depth + 1, :) = (/ 0, x + 1, y, z /)
            stack(depth, 0) = stage + 1
            depth = depth + 1

          else if (stage .eq. 1) then

            stack(depth + 1, :) = (/ 0, x - 1, y, z /)
            stack(depth, 0) = stage + 1
            depth = depth + 1

          else if (stage .eq. 2) then

            stack(depth + 1, :) = (/ 0, x, y + 1, z /)
            stack(depth, 0) = stage + 1
            depth = depth + 1

          else if (stage .eq. 3) then

            stack(depth + 1, :) = (/ 0, x, y - 1, z /)
            stack(depth, 0) = stage + 1
            depth = depth + 1

          else if (stage .eq. 4) then

            stack(depth + 1, :) = (/ 0, x, y, z + 1 /)
            stack(depth, 0) = stage + 1
            depth = depth + 1

          else if (stage .eq. 5) then

            stack(depth + 1, :) = (/ 0, x, y, z - 1 /)
            stack(depth, 0) = stage + 1
            depth = depth + 1

          else if (stage .eq. 6) then

            occupied(x, y, z) = 0
            depth = depth - 1

          end if
        end do

        count_paths = total
      end function
