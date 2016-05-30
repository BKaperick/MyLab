#include "matrix.h"


//Multiplies first two arguments, returns pointer to result
//Should have sparse option
//Returns: result, or null pointer if dimensions do not add up.
matrix* matrix_mult(matrix* m1, matrix* m2) {
	//Check dimension mismatch
	if (m1->cols != m2->rows)
		return NULL;
	matrix *res = malloc(sizeof(matrix));
	matrix_init(res, m1->rows, m2->cols);
	double val;
	for (uint32_t i = 1; i <= m1->rows; i++) {
		for (uint32_t j = 1; j <= m2->cols; j++) {
			val = 0.0;
			for (uint32_t k = 1; k <= m1->cols; k++)
				val += (*matrix_elem(m1,i,k))*(*matrix_elem(m2,k,j));
			matrix_insert(res,i,j,val);
		}
	}
	return res;	
}

//Adds first two arguments, returns pointer to result. 
//Returns: result, or null pointer if dimensions do not add up.
matrix* matrix_add(matrix* m1, matrix* m2) {
	
	//check dimensionality
	if (m1->rows != m2->rows || m1->cols != m2->cols)
		return NULL;

	//initialize output
	matrix *res = malloc(sizeof(matrix));
	matrix_init(res, m1->rows, m1->cols);
	
	//perform addition
	for (uint32_t r = 1; r <= m1->rows; r++) {
		for (uint32_t c = 1; c <= m1->cols; c++) {
			*matrix_elem(res, r, c) = (*matrix_elem(m1, r, c)) + (*matrix_elem(m2, r, c));
		}
	}
	return res;
}

//Subtracts the second from the first argument, storing result.
//Returns: result, or null pointer if dimensions don't add up.
matrix* matrix_sub(matrix* m1, matrix* m2) {
	
	//check dimensionality
	if (m1->rows != m2->rows || m1->cols != m2->cols)
		return NULL;

	//initialize output
	matrix *res = malloc(sizeof(matrix));
	matrix_init(res, m1->rows, m1->cols);

	//perform subtraction
	for (uint32_t r = 1; r <= m1->rows; r++) {
		for (uint32_t c = 1; c <= m1->cols; c++) {
			*matrix_elem(res, r, c) = (*matrix_elem(m1, r, c)) - (*matrix_elem(m2, r, c));
		}
	}
	return res;
}

//checks if two matrices are equal within a certain tolerance
//Returns: m1 == m2
bool matrix_compare(matrix* m1, matrix* m2, double tol) {

	//Automatically returns false if the dimensions are not the same.
	if (m1->rows != m2->rows || m1->cols != m2->cols) {
		return false;
	}
	//Checks element-wise if any difference is greater than tolerance
	double comp;
	for (uint32_t r = 1; r <= m1->rows; r++) {
		for (uint32_t c = 1; c <= m1->cols; c++) {
			comp = (*matrix_elem(m1, r, c)) - (*matrix_elem(m2, r, c));
			if ((-tol > comp) || (comp > tol)) {
				return false;
			}
		}
	}
	return true;
}
