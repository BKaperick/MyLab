#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include "matrix.h"
#include "frontend.h"
#include "tests.h"

int main() {
	//float t = 100*(testAllAlgebra());	
	//printf("%.2f percent of tests passed\n", t);

	//TESTING
	//parse("define A 2 2\n");
	//parse("define B 2 2\n");
	//parse("define C 2 2\n");
	//parse("define D 2 2\n");
	//parse("A+B+C\n");

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

