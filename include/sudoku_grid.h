#ifndef __S_GRID_H__
#define __S_GRID_H__

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#define EMPTY_ENTRY '-'
#define DEFAULT_SIZE 9

extern int size_N;

typedef struct {
    int value;
 
    // list of 1 or 0
    int* possible_values;
    int num_possible;
} point;

typedef point* (*PtByCellFunc)(point***, int, int);

point* create_point();
void destroy_point(point* pt);

void reset_possible_values(point* pt, int value);

typedef struct {
    int missing_numbers;
    // 2d array of point pointers because why not
    // point triple pointer called points pointing to pointer pointing to point pointer
    point*** points;
    int iteration;
} grid_manager;

point* pt_by_row(point*** points, int row, int i);
point* pt_by_col(point*** points, int col, int i);
point* pt_by_cell(point*** points, int cell, int i);

grid_manager* create_grid(int size);
grid_manager* read_grid_from_file(char* filename);
void _init_grid_repr(grid_manager* grid);
void destroy_grid(grid_manager* grid);

void input_grid(grid_manager* grid);
void display_grid(grid_manager* grid);

void _update_groups_possible_values(point***, PtByCellFunc accessor);
int solve_groups(point***, PtByCellFunc accessor);

int compute_missing_numbers(grid_manager* grid);
int compute_all_possible_points(grid_manager* grid);
int compute_possible_point(grid_manager* grid, int x, int y);
void print_possible_points(point* pt);

int iterate_grid(grid_manager* grid);
int test_point(grid_manager* grid, int x, int y, int value);
int solve_grid(grid_manager* grid);

#endif



