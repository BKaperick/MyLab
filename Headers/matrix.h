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
	double** data;
	bool sparse;
	bool square;
};

typedef struct matrix matrix;


///////////////////////////////////
// Functions in matrix_unary.c
///////////////////////////////////

//Initialize each matrix to the zero matrix initially.
void matrix_init(matrix* mat, uint32_t r, uint32_t c);

//Retrieve element at position mat[r][c] (one-indexed)
//Returns: the retrieved element.
double *matrix_elem(matrix* mat, uint32_t r, uint32_t c);

//Inserts val into mat[r][c]
void matrix_insert(matrix* mat, uint32_t r, uint32_t c, double val);

//Prints the matrix contents, formatted, to stdout
void matrix_print(matrix* mat);

//Scales each element by the scalar
//Returns: pointer to the result
matrix* matrix_scale(matrix* mat, double scale);

//free any memory used in matrix storage
void matrix_free(matrix* mat);

//Returns a pointer to a matrix object which is a shallow copy to the original matrix
//So matrix_segment(&A, 3, 5, 2, 4) returns a pointer to a matrix which is a shallow copy of 
//rows 3-5, columns 2-4, both inclusive.
//Returns: Pointer to shallow copy, or Null if the indices are outside the range of the size of A
matrix* matrix_segment(matrix* mat, uint32_t rStart, uint32_t rEnd, uint32_t cStart, uint32_t cEnd);


///////////////////////////////////
// Functions in matrix_binary.c
///////////////////////////////////

//Multiplies first two arguments, returns pointer to result
//Should have sparse option
//Returns: result, or null pointer if dimensions do not add up.
matrix* matrix_mult(matrix* m1, matrix* m2);

//Adds first two arguments, returns pointer to result. 
//Returns: result, or null pointer if dimensions do not add up.
matrix* matrix_add(matrix* m1, matrix* m2);

//Subtracts the second from the first argument, storing result.
//Returns: result, or null pointer if dimensions don't add up.
matrix* matrix_sub(matrix* m1, matrix* m2);

//checks if two matrices are equal within a certain tolerance
//Returns: m1 == m2
bool matrix_compare(matrix* m1, matrix* m2, double tol);

//Makes a deep copy of from, and stores it in to
//Returns: matrices are same dimension
bool matrix_copy(matrix* from, matrix* to);

//Converts a float to a 1x1 matrix
//Returns: res != NULL
bool convert_scalar(double scalar, matrix* res);

#endif

