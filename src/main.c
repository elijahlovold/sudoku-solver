#include <sudoku_grid.h>


int main(int argc, char** argv) {
    grid_manager* grid = read_grid_from_file("sudoku_grid.txt");

    pt_by_cell(grid->points, 8, 2);

    solve_grid(grid);

    display_grid(grid);

    destroy_grid(grid);
}
