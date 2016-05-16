#include "matrix.h"

//Multiplies first two arguments, stores result in third
//Should have sparse option
//*res should point to an initialized matrix object.
//Returns false if dimensions do not add up.
matrix* matrix_mult(matrix* m1, matrix* m2) {
	//Check dimension mismatch
	if (m1->cols != m2->rows)
		return NULL;
	matrix *res = malloc(sizeof(matrix));
	matrix_init(res, m1->rows, m2->cols);
	for (uint32_t i = 1; i <= m1->rows; i++) {
		for (uint32_t j = 1; j <= m2->cols; j++) {
			double val = 0.0;
			for (uint32_t k = 1; k <= m1->cols; k++) {
				val += (*matrix_elem(m1,i,k))*(*matrix_elem(m2,k,j));
			}
			matrix_insert(res,i,j,val);
		}
	}
	return res;	
}

//Adds first two arguments, returns pointer to result. 
//Returns null pointer if dimensions do not add up.
matrix* matrix_add(matrix* m1, matrix* m2) {
	if (m1->rows != m2->rows || m1->cols != m2->cols)
		return NULL;
	matrix *res = malloc(sizeof(matrix));
	matrix_init(res, m1->rows, m1->cols);
	for (int i = 0; i < m1->size; i++) {
		res->data[i] = m1->data[i] + m2->data[i];
	}
	return res;
}

//Adds first two arguments, returns pointer to result. 
//Returns null pointer if dimensions do not add up.
matrix* matrix_sub(matrix* m1, matrix* m2) {
	if (m1->rows != m2->rows || m1->cols != m2->cols)
		return NULL;
	matrix *res = malloc(sizeof(matrix));
	matrix_init(res, m1->rows, m1->cols);
	for (int i = 0; i < m1->size; i++) {
		res->data[i] = m1->data[i] - m2->data[i];
	}
	return res;
}

//checks if two matrices are equal within a certain tolerance
bool matrix_compare(matrix* m1, matrix* m2, double tol) {

	//Automatically returns false if the dimensions are not the same.
	if (m1->rows != m2->rows || m1->cols != m2->cols) {
		return false;
	}
	//Checks element-wise if any difference is greater than tolerance
	for (int i = 0; i < m1->size; i++) {
		double comp = m1->data[i] - m2->data[i];
		if ((-tol > comp) || (comp > tol)) {
			return false;
		}
	}
	return true;
}
