#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include "matrix.h"
#include "frontend.h"


bool test_matrix_mult(double tol) {
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
	matrix* ans = matrix_mult(&m1, &m2);
	bool result = matrix_compare(&correct, ans, tol);
	if (!result) {
		matrix_print(&m1);
		matrix_print(&m2);
		matrix_print(ans);
	}	
	matrix_free(&m1);
	matrix_free(&m2);
	matrix_free(&correct);
	matrix_free(ans);
	return result;
}

int test_matrix_add_subtract(double tol) {
	matrix m1, m2, correct, subcorrect;
	matrix_init(&m1, 2, 2);
	matrix_init(&m2, 2, 2);
	matrix_init(&correct, 2, 2);
	matrix_init(&subcorrect, 2, 2);

	matrix_insert(&m1, 1, 1, 1.0);
	matrix_insert(&m1, 1, 2, 2.0);
	matrix_insert(&m1, 2, 1, 3.0);
	matrix_insert(&m1, 2, 2, 4.0);
	matrix_insert(&m2, 1, 1, 5.0);
	matrix_insert(&m2, 1, 2, 6.0);
	matrix_insert(&m2, 2, 1, 7.0);
	matrix_insert(&m2, 2, 2, 8.0);
	matrix_insert(&correct, 1, 1, 6.0);
	matrix_insert(&correct, 1, 2, 8.0);
	matrix_insert(&correct, 2, 1, 10.0);
	matrix_insert(&correct, 2, 2, 12.0);
	matrix_insert(&subcorrect, 1, 1, -4.0);
	matrix_insert(&subcorrect, 1, 2, -4.0);
	matrix_insert(&subcorrect, 2, 1, -4.0);
	matrix_insert(&subcorrect, 2, 2, -4.0);

	matrix* ans = matrix_add(&m1, &m2);
	matrix* sub_ans = matrix_sub(&m1, &m2);
	int result = matrix_compare(&correct, ans, tol);
	int other_result = matrix_compare(sub_ans, &subcorrect, tol);
	matrix_free(&m1);
	matrix_free(&m2);
	matrix_free(&correct);
	matrix_free(&subcorrect);
	matrix_free(sub_ans);
	matrix_free(ans);
	return result + other_result;
}

int main() {
	int tests = 0;
	tests += test_matrix_mult(.01);
	tests += test_matrix_add_subtract(.01);
	printf("%d / %d tests passed\n", tests, 3);

	char input[MAX_INPUT_SIZE];// = malloc(INPUT_SIZE * sizeof(char));
	while (strcmp("exit\n", input) != 0) {
		printf("> ");
		fgets(input, MAX_INPUT_SIZE, stdin);
		bool syntax_check = parse(input);
		if (!syntax_check) {
			printf("INVALID SYNTAX\n");
		}
	}

	return 1;
}

