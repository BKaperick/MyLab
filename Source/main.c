#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include "matrix.h"
#include "frontend.h"

int main() {
	char input[MAX_INPUT_SIZE];// = malloc(INPUT_SIZE * sizeof(char));
    bool syntax_check;
    FILE *fp;
    char* filename;
	while (strcmp("exit\n", input) != 0) {
		printf("> ");
		fgets(input, MAX_INPUT_SIZE, stdin);
		if (input[0] == 'r' && input[1] == 'u' && input[2] == 'n' && input[3] == ' ') {
            filename = input+4;
            filename[strlen(filename)-1] = 0;
            char line[MAX_INPUT_SIZE];
            fp = fopen(filename, "r");
            while (fgets(line, sizeof line, fp)) {
                syntax_check = parse(line);
                if (!syntax_check) {
                    printf("-- INVALID SYNTAX\n");
                }
            }
            if (!syntax_check)
                printf("FILE DOES NOT EXIST ");
        }
        else {
            syntax_check = parse(input);
            if (!syntax_check) {
                printf("-- INVALID SYNTAX\n");
            }
        }
	}
	variable_free();

	return 1;
}

