#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

struct matrix {
	uint32_t rows;
	uint32_t cols;
	uint64_t size;
	double* data;
	bool sparse;
	bool square;
};

typedef struct matrix matrix;


//Initialize each matrix to the zero matrix initially.
void matrix_init(matrix* mat, uint32_t r, uint32_t c);

//Retrieve element at position mat[r][c] (one-indexed
double *matrix_elem(matrix* mat, uint32_t r, uint32_t c);

void matrix_insert(matrix* mat, uint32_t r, uint32_t c, double val);

//Multiplies first two arguments, stores result in third
//Should have sparse option
//*res should point to an initialized matrix object.
//Returns false if dimensions do not add up.
bool matrix_mult(matrix* m1, matrix* m2, matrix* res);

//Adds first two arguments, stores result in third. 
//Returns false if dimensions do not add up.
bool matrix_add(matrix* m1, matrix* m2, matrix* res);

//Prints the matrix contents, formatted, to stdout
void matrix_print(matrix* mat);

//free any memory used in matrix storage
void matrix_free(matrix* mat);

#endif

