#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include "matrix.h"
#include "frontend.h"

int main() {
	char input[MAX_INPUT_SIZE];
	
    //Stack of commands to execute
    char** execute_queue = malloc(10*sizeof(char*));//malloc(MAX_SCRIPT_SIZE*MAX_SCRIPT_DEPTH*sizeof(char*));
    int eq_start = 0;
    int eq_end = 0;

    do {
        //Get input from user
        printf("> ");
        fgets(input, MAX_INPUT_SIZE, stdin);
        eq_end++;

	    while (eq_end - eq_start != 0) {
            execute_statement(input, execute_queue, &eq_end);
            eq_start++; 
        }
    }
    while (strcmp(input, "exit\n") != 0);

	variable_free();

	return 1;


}

