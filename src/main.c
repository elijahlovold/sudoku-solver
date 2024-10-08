#include <sudoku_grid.h>

int main(int argc, char** argv) {
    int size_N = 9;

    if (argc > 1) {
        size_N = atoi(argv[1]);
        printf("running for %d\n", size_N);
        if (size_N < 2) {
            printf("Error, size cannot be less than two\n");
            printf("Aborting solver...");
            return -1;
        }
    }

    grid_manager* grid = create_grid(size_N);

    input_grid(grid);
    display_grid(grid);

    printf("number remaining: %d", compute_missing_numbers(grid));






    destroy_grid(grid);
}
