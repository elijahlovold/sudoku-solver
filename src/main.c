#include <sudoku_grid.h>


int main(int argc, char** argv) {
    grid_manager* grid;
    if (argc > 1) {
        grid = read_grid_from_file(argv[1]);
    }
    else {
        char input[100];

        printf("Please specify grid file: ");
        scanf("%99s", input);
        grid = read_grid_from_file(input);
    }
    if (!grid) {
        return 0;
    }

    solve_grid(grid);

    display_grid(grid);

    destroy_grid(grid);
}
