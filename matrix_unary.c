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

//Retrieve element at position mat[r][c] (one-indexed)
//Returns: the retrieved element.
double *matrix_elem(matrix* mat, uint32_t r, uint32_t c) {
	uint64_t index = (r-1)*(mat->cols) + c - 1;
	double *res = &(mat->data[index]);
	return res;
}

//Inserts val into mat[r][c]
void matrix_insert(matrix* mat, uint32_t r, uint32_t c, double val) {
	double* temp = matrix_elem(mat, r, c);
	*temp = val;
}


//Prints the matrix contents, formatted, to stdout
void matrix_print(matrix* mat) {
	for (int32_t i = 1; i <= mat->rows; i++) {
		printf("[ ");
		for (int32_t j = 1; j <= mat->cols; j++) {
			printf("%4f", *matrix_elem(mat, i, j));
			printf(" ");
		}
		printf("]\n");
	}
}

//Scales each element by the scalar
//Returns: pointer to the result
matrix* matrix_scale(matrix* mat, double scale) {
	matrix* res = malloc(sizeof(matrix));
	matrix_init(res, mat->rows, mat->cols);
	for (int i = 0; i < mat->size; i++) {
		res->data[i] = mat->data[i]*scale;
	}
	return res;
}

//free any memory used in matrix storage
void matrix_free(matrix* mat) {
	free(mat->data);
}

