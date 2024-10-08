#ifndef __S_GRID_H__
#define __S_GRID_H__

#include <stdio.h>
#include <stdlib.h>

#define EMPTY_ENTRY '-'
#define DEFAULT_SIZE 9

extern int size_N;

typedef struct {
    int value;
    int* possible_values;
} point;

point* create_point();
void destroy_point(point* pt);

void reset_possible_values(point* pt);

typedef struct {
    int missing_numbers;
    // 2d array of point pointers because why not
    point*** points;
} grid_manager;

grid_manager* create_grid(int size);
void destroy_grid(grid_manager* grid);

void input_grid(grid_manager* grid);
void display_grid(grid_manager* grid);

int compute_missing_numbers(grid_manager* grid);
void iterate_grid(grid_manager* grid);
int test_point(grid_manager* grid, int x, int y, int value);

#endif



