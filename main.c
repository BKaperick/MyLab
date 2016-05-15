#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include "matrix.h"

int main() {
	matrix m;
	matrix_init(&m, 5, 5);
	matrix_print(&m);
	matrix_insert(&m, 2, 2, 3.0);
	printf("rand %f\n", m.data[3]);
	matrix_print(&m);
	printf("rand %f\n", m.data[3]);
	matrix_free(&m);
	return 1;
}
