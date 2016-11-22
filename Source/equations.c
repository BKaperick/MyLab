#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

#include "matrix.h"
#include "frontend.h"
#include "equations.h"

//>0 if c1 > c2
int comp(char* c1, char* c2) {
	if (!c2)
		return 2;
	if (strcmp(c1,"==") == 0) {
		if (strcmp(c2, "==") == 0) {return 0;}
		else{	return 1;}
	}
	if (strcmp(c2,"==") == 0) {
		return -1;
	}
	if (strcmp(c1, "*") == 0) {
		if (strcmp(c2, "*") == 0)	{return 0;}
		else{	return 1;}
	}
	if (strcmp(c2, "*") ==0) {
		return -1;
	}
	else	{return 0;}	
}
bool op(char* c) {
	return (strcmp(c, "==") == 0 || strcmp(c, "+") == 0 || strcmp(c, "-") == 0 || strcmp(c, "*") == 0);
}

bool paren(char* c) {
	return (strcmp(c, "(") == 0 || strcmp(c, ")") == 0);
}


void print_input(char** input, int len) {
	printf("[");
	if (len > 0) {
		for (int i = 0; i < len; i++) {
			printf("%s, ", input[i]);
		}
	}
	printf("]\n");
}

void print_all_inds(char** input, int in_ind, char** output, int out_ind, char** deck, int deck_ind) {
	printf("v------------------------------\n");
	print_input(input, in_ind);
	print_input(output, out_ind);
	print_input(deck, deck_ind);
	printf("^------------------------------\n");
}

char** postfix(char** input, int len) {
	char* c = input[0];
	char** output = calloc(len, sizeof(char*));
	int out_ind = 0;
	int in_ind = 1;
	char** deck = calloc(len, sizeof(char*));
	int deck_ind = 0;
	while ( c && in_ind <= len) {
		//print_all_inds(input, in_ind, output, out_ind, deck, deck_ind);
		if (op(c)) {
			//always just add operators to the deck
			deck[deck_ind] = c;
			deck_ind++;
			c = input[in_ind];
			in_ind++;
		}
		else if (paren(c)) {
			int depth = 1;
			char** chunk;
			int chunkLen = 0;
			//number of spaces to skip in_ind ahead
			int skipAhead = 1;
			for (int ind = in_ind; ind < len; ind++) {
				char* current = input[ind];
				if (strcmp(current, "(") == 0) {
					depth++;
				}
				else if (strcmp(current, ")") == 0) {
					depth--;
				}
				else
					chunkLen++;
				if (depth == 0) {
					skipAhead = ind - in_ind + 1;
					break;
				}
			}
			chunk = postfix(&input[in_ind], skipAhead-1);
			for (int i = 0; i < chunkLen; i++) {
				output[out_ind] = chunk[i];
				out_ind++;
			}
			in_ind += skipAhead;
			printf("in_ind = %d\nchunkLen = %d\nskipAhead = %d\n", in_ind, chunkLen, skipAhead);
			//Add in everything relevant onto deck and output from within the parentheses
			//while something on deck, order of ops still in favor
			while (deck_ind > 0 && (in_ind == len || comp(deck[deck_ind-1], input[in_ind])) >= 0) {
				//put top of deck on output
				output[out_ind] = deck[deck_ind-1];
				deck_ind--;
				out_ind++;
			}
			c = input[in_ind];
			in_ind++;
		}
		
		//Variable name or constant
		else {
			output[out_ind] = c;
			out_ind++;
			
			while (deck_ind > 0 && (in_ind == len || comp(deck[deck_ind-1], input[in_ind]) >= 0)) {
					//Add top of deck to output
					output[out_ind] = deck[deck_ind-1];
					deck_ind--;
					out_ind++;
			}
			c = input[in_ind];
			in_ind++;
		}
		if (in_ind > len) {
			break;
		}
	}
	in_ind--;
	//printf("(%d, %d, %d)\n", in_ind, out_ind, deck_ind);
	//print_all_inds(input, in_ind, output, out_ind, deck, deck_ind);
	
	if (deck_ind > 0) {
		printf("should never be called\n");
		for (int i = deck_ind-1; i >= 0; i--) {
			output[out_ind] = deck[i];
			//was -- should be ++
			out_ind++;
		}
	}
	free(deck);
	return output;		
}

matrix* operate(matrix* val1, matrix* val2, char* op) {
	if (strcmp(op, "+") == 0)
		return matrix_add(val1, val2);
	else if (strcmp(op, "-") == 0)
		return matrix_sub(val1, val2);
	else if (strcmp(op, "*") == 0)
		return matrix_mult(val1, val2);
	else if (strcmp(op, "==") == 0) {
		double res = matrix_compare(val1, val2, .0001)?1.0:0.0;
		matrix* out = malloc(sizeof(matrix));
	       	convert_scalar(res, out);
		return out;
	}	
	else 
		return NULL;
}

bool evaluateEq(char** eq, int len, matrix* outputPtr) {	
	eq = postfix(eq, len);
	for (int i = 0; i < len; i++) {
		if (!(eq[i])) {
			len = i;
		}
	}
	//Stack on which variables are temporarily stored
	matrix** varstack = calloc(len, sizeof(matrix*));
	
	//Index in the equation being evaluated
	int v_ind = 0;

	bool firstVar = true;

	//Iterate over string.  Algorithm is performed in a single simple pass, so i
	//is only changed by ticking one each loop
	for (int i = 0; i < len; i++) {
		//If the character is an operator
		if (op(eq[i])) {
			//Perform operation on two top values of stack
			matrix* temp = operate(varstack[v_ind-2], varstack[v_ind-1], eq[i]);
			//outputPtr = matrix_add(temp, outputPtr);
			
			//Move stack pointer one place down
			v_ind --;
			//Add the resultant onto the top of the stack
			varstack[v_ind-1] = temp;
		}
		//If the character is a variable name
		else {
			matrix* v = variable_get_matrix(eq[i]);

			//Try to make it into a scalar if the variable does not exist
			if (v == NULL) {
				v = malloc(sizeof(matrix));
				double elem = strtod(eq[i], NULL);
				matrix_init(v,1,1);
				v->data[0][0] = elem;
			}
			if (firstVar) {
				firstVar = false;

				//it's a simple assignment.
				if (len == 1) {
					return matrix_copy(v, outputPtr);
				}
			
			}
			//Simply add character to stack
			varstack[v_ind] = v;
			v_ind++;
		}
	}
	
	//Copy final answer over to output
	matrix_copy(varstack[0], outputPtr);
	//Stack is no longer needed
	free(varstack);
	return true;
}
