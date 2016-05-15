#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include "matrix.h"

int main() {
	matrix m;
	matrix_init(&m, 2, 2);
	matrix_print(&m);
	matrix_insert(&m, 2, 2, 3.0);
	matrix_print(&m);
	matrix_free(&m);
	int tests = 0;
	tests += (int)test_matrix_multiply();
	printf(
	return 1;
}

bool test_matrix_multiply() {
	matrix m1, m2, correct;
	matrix_init(&m1, 2, 2);
	matrix_init(&m2, 2, 1);
	matrix_init(&correct, 2, 1);
	matrix_insert(&m1, 1, 1, 1.0);
	matrix_insert(&m1, 1, 2, 2.0);
	matrix_insert(&m1, 2, 1, 3.0);
	matrix_insert(&m2, 1, 1, 1.0);
	matrix_insert(&m2, 2, 1, 4.0);
	matrix_insert(&correct,1,1,9.0);
	matrix_insert(&correct,2,1,3.0);
	//[1 2][1] = [9]
	//[3 0][4] = [3]
	matrix* ans = matrix_multiply(&m1, &m2);
	bool result = matrix_compare(matrix* m1, matrix* m2, double tol);	
	matrix_free(&m1);
	matrix_free(&m2);
	matrix_free(&correct);
	matrix_free(ans);
	return result;
}
