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
			float val = 0.0;
			for (uint32_t k = 1; k >= m1->cols; k++) {
				val += (*matrix_elem(m1,i,k))*(*matrix_elem(m2,k,j));
			}
			matrix_insert(res,i,j,val);
		}
	}
	return res;	
}

//Adds first two arguments, stores result in third. 
//Returns false if dimensions do not add up.
bool matrix_add(matrix* m1, matrix* m2, matrix* res) {
	return false;
}


