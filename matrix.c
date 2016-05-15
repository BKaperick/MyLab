#include "matrix.h"

//Initialize each matrix to the zero matrix initially.
void matrix_init(matrix* mat, uint32_t r, uint32_t c) {
	mat->rows = r;
	mat->cols = c;
	mat->data = calloc(r*c, sizeof(double));
	mat->size = r*c;

	//By default, matrices are sparse since they are zero matrices.
	mat->sparse = true;
	mat->square = (r == c);
}

//Retrieve element at position mat[r][c] (one-indexed
float *matrix_elem(matrix* mat, uint32_t r, uint32_t c) {
	uint64_t index = (r-1)*(mat->cols) + c - 1;
	float *res = &(mat->data[index]);
	return res;
}

void matrix_insert(matrix* mat, uint32_t r, uint32_t c, float val) {
	*matrix_elem(mat, r, c) = val;
}

//Multiplies first two arguments, stores result in third
//Should have sparse option
//*res should point to an initialized matrix object.
//Returns false if dimensions do not add up.
bool matrix_mult(matrix* m1, matrix* m2, matrix* res) {
	return false;
}

//Adds first two arguments, stores result in third. 
//Returns false if dimensions do not add up.
bool matrix_add(matrix* m1, matrix* m2, matrix* res) {
	return false;
}

void matrix_print(matrix* mat) {
	printf("\n");
	for (int64_t i = 0; i < mat->rows; i++) {
		printf("[ ");
		for (int64_t j = 0; j < mat->cols; j++) {
			printf("%4f", matrix_elem(mat, i, j));
			printf(" ");
		}
		printf("]\n");
	}
}
