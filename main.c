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
	matrix_insert(&m, 1, 1, 3.0);
	matrix_print(&m);
	return 1;
}
