#include <sudoku_grid.h>

int size_N;

point* create_point() {
    point* pt = (point*)malloc(sizeof(point));
    if (pt == NULL) {
        perror("Failed to allocate memory for point");
        return NULL; // Handle allocation failure
    }

    pt->value = -1;
    pt->possible_values = (int *)malloc(size_N * sizeof(int));
    if (pt->possible_values == NULL) {
        free(pt->possible_values); // Clean up previously allocated memory
        perror("Failed to allocate memory for data");
        return NULL; // Handle allocation failure
    }

    // Initialize array elements to zero
    reset_possible_values(pt);
    return pt;
}

// Destructor: Cleans up the DynamicArray
void destroy_point(point* pt) {
    if (pt != NULL) {
        free(pt->possible_values); // Free the allocated data
        free(pt);       // Free the struct itself
    }
}

void reset_possible_values(point* pt) {
    for (int i = 0; i < size_N; i++) {
        pt->possible_values[i] = i;
    }
};

grid_manager* create_grid(int size) {
    if (size < 2) {
        printf("Error, size cannot be less than two");
        printf("Using default value of %d", DEFAULT_SIZE);
        size_N = DEFAULT_SIZE;
    }
    size_N = size;

    grid_manager* grid = (grid_manager*)malloc(sizeof(grid_manager));
    if (grid == NULL) {
        perror("Failed to allocate memory for point");
        return NULL; // Handle allocation failure
    }

    // start missing all
    grid->missing_numbers = size_N*size_N;

    point** points;
    grid->points = (point***)malloc(size_N * sizeof(point**));
    if (grid->points == NULL) {
        free(grid->points); // Clean up previously allocated memory
        perror("Failed to allocate memory for points on grid");
        return NULL; // Handle allocation failure
    }

    for (int i = 0; i < size_N; i++) {
        grid->points[i] = (point**)malloc(size_N * sizeof(point*));
        if (grid->points[i] == NULL) {
            free(grid->points[i]); // Clean up previously allocated memory
            perror("Failed to allocate memory for points on grid");
            return NULL; // Handle allocation failure
        }

        for (int j = 0; j < size_N; j++) {
            grid->points[i][j] = create_point();
        }
    } 

    return grid;
} 


void destroy_grid(grid_manager* grid) {
    for (int i = 0; i < size_N; i++) {
        for (int j = 0; j < size_N; j++) {
            destroy_point(grid->points[i][j]);
        }
        free(grid->points[i]);
    } 
    free(grid->points);

    free(grid);
}

void input_grid(grid_manager* grid) {
    printf("Enter the current sudoku grid (%d x %d):\n", size_N, size_N);
    for (int i = 0; i < size_N; i++) {
        for (int j = 0; j < size_N; j++) {
            char values[10];
            scanf(" %c", values); // Use " %c" to ignore whitespace

            if (values[0] == EMPTY_ENTRY || values[0] == '_') {
                grid->points[i][j]->value = -1;
            } else {
                int value = atoi(values);
                grid->points[i][j]->value = value;
            }
        }
    }
}

void display_grid(grid_manager* grid) {
    printf("Sudoku grid:\n");
    for (int i = 0; i < size_N; i++) {
        for (int j = 0; j < size_N; j++) {
            int value = grid->points[i][j]->value;
            if (value != -1) {
                printf("%d ", value);
            } else {
                printf("- "); // Print non-numeric characters as is
            }
        }
        printf("\n");
    } 
}

int compute_missing_numbers(grid_manager* grid) {
    int num = 0;
    for (int i = 0; i < size_N; i++) {
        for (int j = 0; j < size_N; j++) {
            if (grid->points[i][j]->value == -1) {
                num++;
            }
        }
    }

    grid->missing_numbers = num;
    return num;
};
void iterate_grid(grid_manager* grid);
int test_point(grid_manager* grid, int x, int y, int value);

