#include <sudoku_grid.h>

int size_N;

point* create_point() {
    point* pt = (point*)malloc(sizeof(point));
    if (pt == NULL) {
        perror("Failed to allocate memory for point\n");
        return NULL; // Handle allocation failure
    }

    pt->value = -1;
    pt->possible_values = (int *)malloc(size_N * sizeof(int));
    if (pt->possible_values == NULL) {
        free(pt->possible_values); // Clean up previously allocated memory
        perror("Failed to allocate memory for data\n");
        return NULL; // Handle allocation failure
    }

    // Initialize array elements to zero
    reset_possible_values(pt, 1);
    return pt;
}

// Destructor: Cleans up the DynamicArray
void destroy_point(point* pt) {
    if (pt != NULL) {
        free(pt->possible_values); // Free the allocated data
        free(pt);       // Free the struct itself
    }
}

void reset_possible_values(point* pt, int value) {
    for (int i = 0; i < size_N; i++) {
        pt->possible_values[i] = value;
    }
    if (value == 1) {
        pt->num_possible = size_N;
    } else {
        pt->num_possible = 0;
    }
};

grid_manager* create_grid(int size) {
    if (size < 2) {
        printf("Error, size cannot be less than two\n");
        printf("Using default value of %d\n", DEFAULT_SIZE);
        size_N = DEFAULT_SIZE;
    }

    double root = sqrt((double)size);
    if (root * root != size) {
        printf("Error, size must be perfect root: %d\n", size);
        return NULL;
    }
    size_N = size;

    printf("creating grid size: %d\n", size_N);
    grid_manager* grid = (grid_manager*)malloc(sizeof(grid_manager));
    if (grid == NULL) {
        perror("Failed to allocate memory for point\n");
        return NULL; // Handle allocation failure
    }

    // start missing all
    grid->missing_numbers = size_N*size_N;

    point** points;
    grid->points = (point***)malloc(size_N * sizeof(point**));
    if (grid->points == NULL) {
        free(grid->points); // Clean up previously allocated memory
        perror("Failed to allocate memory for points on grid\n");
        return NULL; // Handle allocation failure
    }

    for (int i = 0; i < size_N; i++) {
        grid->points[i] = (point**)malloc(size_N * sizeof(point*));
        if (grid->points[i] == NULL) {
            free(grid->points[i]); // Clean up previously allocated memory
            perror("Failed to allocate memory for points on grid\n");
            return NULL; // Handle allocation failure
        }

        for (int j = 0; j < size_N; j++) {
            grid->points[i][j] = create_point();
        }
    } 

    grid->iteration = 0;
    return grid;
} 

grid_manager* read_grid_from_file(char* filename) {
    FILE *file = fopen(filename, "r"); // Open file in read mode
                                                //
    if (file == NULL) {                      // Check if file opened successfully
        perror("Error opening file");
        return NULL;
    }

    int size_N = fgetc(file) - '0';
    grid_manager* grid = create_grid(size_N);
    if (grid == NULL) {
        return NULL;
    }

    char ch;
    int i = 0, j = 0;
    while ((ch = fgetc(file)) != EOF) {
        int iter = 0;
        if (ch == EMPTY_ENTRY || ch == '_') {
            grid->points[i][j]->value = -1;
            iter = 1;
        } else if (isdigit(ch)) {
            int num = ch - '0';
            if (num > size_N || num < 1) {
                printf("error, %d is invalid. Must be between -1 and %d", num, size_N); 
                return NULL;
            }
            grid->points[i][j]->value = num;
            iter = 1;
        }

        if (iter) {
            j++;
            if (j >= size_N) {
                j = 0;
                i++;
            }
            if (i >= size_N) {
                break;
            }
        }
    }

    fclose(file); // Close file

    printf("grid read successfully\n\n");
    display_grid(grid);
    return grid;
}

void destroy_grid(grid_manager* grid) {
    if (grid == NULL) {
        return;
    }

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
                if (value > size_N || value < 1) {
                    printf("error, %d is invalid. Must be between -1 and %d\n", value, size_N); 
                    return;
                }
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

int compute_all_possible_points(grid_manager* grid) {
    int num_updated = 0;
    for (int i = 0; i < size_N; i++) {
        for (int j = 0; j < size_N; j++) {
            num_updated += compute_possible_point(grid, i, j);
        }
    }
    return num_updated;
}

int compute_possible_point(grid_manager* grid, int x, int y) {
    // if not -1, point is already found!
    if (grid->points[x][y]->value != -1) {
        grid->points[x][y]->num_possible = 1;
        return 0;
    }

    int num_possible = size_N;
    for (int i = 0; i < size_N; i++) {
        if (i != x) {
            int value = grid->points[i][y]->value;
            if (value != -1 && grid->points[x][y]->possible_values[value - 1] != 0) {
                // remove this point by setting to 0 
                grid->points[x][y]->possible_values[value - 1] = 0;
                num_possible--;
            }
        }
        if (i != y) {
            int value = grid->points[x][i]->value;
            if (value != -1 && grid->points[x][y]->possible_values[value - 1] != 0) {
                // remove this point by setting to 0 
                grid->points[x][y]->possible_values[value - 1] = 0;
                num_possible--;
            }
        }
    }

    grid->points[x][y]->num_possible = num_possible;
    if (num_possible == 1) {
        for (int i = 0; i < size_N; i++) {
            int value = grid->points[x][y]->possible_values[i];
            if (value == 1) {
                grid->points[x][y]->value = i + 1;
                return 1;
            }
        }
    }
    return 0;
}

void print_possible_points(point* pt) {
    printf("Possible numbers: ");

    int printed = 0;
    for (int i = 0; i < size_N; i++) {
        if (pt->possible_values[i] == 1) {
            printf("%d, ", i + 1);
            printed = 1;

        }
    }
    if (printed == 0) {
        printf("None\n");
    }
    printf("\n");
}

int iterate_grid(grid_manager* grid) {
    printf("iter: %d\n", grid->iteration);

    int num_direct_infer = compute_all_possible_points(grid);
    grid->iteration++;

    return num_direct_infer;
}

int test_point(grid_manager* grid, int x, int y, int value);

int solve_grid(grid_manager* grid) {
    int num_changed;
    do {
        num_changed = iterate_grid(grid);
    } while(num_changed != 0);

    // only win if there are no more
    return compute_missing_numbers(grid) == 0;
}
