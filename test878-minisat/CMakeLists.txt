cmake_minimum_required(VERSION 3.5)
project(test878-minisat LANGUAGES CXX)

set(CMAKE_CXX_EXTENSIONS OFF)

find_package(MiniSat 2.2 REQUIRED)

add_executable(
    main
    main.cc
)
target_link_libraries(main MiniSat::libminisat)
