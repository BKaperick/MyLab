#include "matrix.h"

//Initialize each matrix to the zero matrix initially.
void matrix_init(matrix* mat, uint32_t r, uint32_t c) {
	mat->rows = r;
	mat->cols = c;
	mat->data = calloc(r, sizeof(double*));
	for (int i = 0; i < r; i++) {
		mat->data[i] = calloc(c, sizeof(double));
	}
	for (int i = 0; i < r; i++) {
		if (i < c) {
			mat->data[i][i] = 1;
		}
		else
			break;
	}
	mat->size = r*c;

	//By default, matrices are sparse since they are zero matrices.
	mat->sparse = true;
	mat->square = (r == c);
}

//Retrieve element at position mat[r][c] (one-indexed)
//Returns: the retrieved element.
double *matrix_elem(matrix* mat, uint32_t r, uint32_t c) {
	//uint64_t index = (r-1)*(mat->cols) + c - 1;
	double *res = &(((mat->data)[r-1])[c-1]);
	//double *res = &(mat->data[index]);
	return res;
}

//Inserts val into mat[r][c]
void matrix_insert(matrix* mat, uint32_t r, uint32_t c, double val) {
	*(matrix_elem(mat, r, c)) = val;
	//*temp = val;
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
	for (uint32_t r = 0; r < mat->rows; r++) {
		for (uint32_t c = 0; c < mat->cols; c++) {
			*matrix_elem(res, r, c) = scale*(*matrix_elem(mat, r, c));
		}
	}
	return res;
}

//free any memory used in matrix storage
void matrix_free(matrix* mat) {
	free(mat->data);
}

//Returns a pointer to a matrix object which is a shallow copy to the original matrix
//So matrix_segment(&A, 3, 5, 2, 4) returns a pointer to a matrix which is a shallow copy of 
//rows 3-5, columns 2-4, both inclusive.
//Returns: Pointer to shallow copy, or Null if the indices are outside the range of the size of A
matrix* matrix_segment(matrix* mat, uint32_t rStart, uint32_t rEnd, uint32_t cStart, uint32_t cEnd) {
	matrix* res = malloc(sizeof(matrix));
	matrix_init(res, rEnd - rStart + 1, cEnd - cStart + 1);
	for (uint32_t r = 0; r < res->rows; r++) {
		res->data[r] = mat->data[r + rStart - 1] + cStart - 1;
	}
	return res;
}

bool convert_scalar(double scalar, matrix* res) {
	if (res == NULL)
		return false;
	matrix_init(res, 1, 1);
	*matrix_elem(res, 1, 1) = scalar;
	return true;
}
