#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include "matrix.h"
#include "frontend.h"

int main() {
	char input[MAX_INPUT_SIZE];// = malloc(INPUT_SIZE * sizeof(char));
	while (strcmp("exit\n", input) != 0) {
		printf("> ");
		fgets(input, MAX_INPUT_SIZE, stdin);
		bool syntax_check = parse(input);
		if (!syntax_check) {
			printf("-- INVALID SYNTAX\n");
		}
	}
	variable_free();

	return 1;
}

