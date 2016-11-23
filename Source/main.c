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
    //char* test_str = "testing";
    //execute_queue[0] = test_str;
    do {
        //Get input from user
        printf("> ");
        execute_queue[eq_start] = malloc(MAX_INPUT_SIZE*sizeof(char*));
        fgets(execute_queue[eq_start], MAX_INPUT_SIZE, stdin);
        eq_end++;

        //Execute statements while there is still more waiting.
	    while (eq_end - eq_start != 0) {
            execute_statement(execute_queue[eq_start], &execute_queue, &eq_end);
            eq_start++; 
        }
    }
    while (strcmp(input, "exit\n") != 0);

	variable_free();

	return 1;


}

