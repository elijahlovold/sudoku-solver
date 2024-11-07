cmake_minimum_required(VERSION 3.13)

project(sudoku_solver C)
set(CMAKE_C_STANDARD 17)

add_executable(sudoku_solver)

target_include_directories(sudoku_solver PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include/)

target_sources(sudoku_solver PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/src/main.c
        ${CMAKE_CURRENT_LIST_DIR}/src/sudoku_grid.c
        )

target_link_libraries(sudoku_solver m)           # Link the math library
