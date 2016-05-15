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
double *matrix_elem(matrix* mat, uint32_t r, uint32_t c) {
	uint64_t index = (r-1)*(mat->cols) + c - 1;
	double *res = &(mat->data[index]);
	return res;
}

void matrix_insert(matrix* mat, uint32_t r, uint32_t c, double val) {
	double* temp = matrix_elem(mat, r, c);
	*temp = val;
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
	for (int32_t i = 1; i <= mat->rows; i++) {
		printf("[ ");
		for (int32_t j = 1; j <= mat->cols; j++) {
			printf("%4f", *matrix_elem(mat, i, j));
			printf(" ");
		}
		printf("]\n");
	}
}

//free any memory used in matrix storage
void matrix_free(matrix* mat) {
	free(mat->data);
}
