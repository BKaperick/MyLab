#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include "matrix.h"
#include "frontend.h"

int main() {
	char input[MAX_INPUT_SIZE];
	
    do {
        //Get input from user
        printf("> ");
        fgets(input, MAX_INPUT_SIZE, stdin);

        //Execute statements while there is still more waiting.
        execute_statement(input);
    }
    while (strcmp(input, "exit\n") != 0);

	variable_free();

	return 1;


}

