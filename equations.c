#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

#include "matrix.h"
#include "frontend.h"

//>0 if c1 > c2
int comp(char* c1, char* c2) {
	if (!c2)
		return 2;
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
	return (strcmp(c, "+") == 0 || strcmp(c, "-") == 0 || strcmp(c, "*") == 0);
}

bool paren(char* c) {
	return (strcmp(c, "(") == 0 || strcmp(c, ")") == 0);
}

char** postfix(char** input, int len) {
	char* c = input[0];
	char** output = calloc(len, sizeof(char*));
	int out_ind = 0;
	int in_ind = 1;
	char** deck = calloc(len, sizeof(char*));
	int deck_ind = 0;
	while ( c && in_ind <= len) {
		//printf("\tchar \"%c\"\n", c);
		if (op(c)) {
			//printf("\t\toperator put on deck\n");
			//always just add operators to the deck
			deck[deck_ind] = c;
			deck_ind++;
			c = input[in_ind];
			in_ind++;
		}
		else if (paren(c)) {
			int depth = 1;
			char** chunk;// = calloc(len - in_ind, sizeof(char));
			int len = 0;
			int chunk_len = 0;
			char* exp = input[in_ind];
			while (depth >= 1) {
				if (strcmp(exp, "(") == 0)
					depth++;
				else if (strcmp(exp, ")") == 0)
					depth--;
				if (depth != 0) {
				len++;
				exp = input[in_ind + len];
				}
				if (!paren(exp))
					chunk_len++;
			}
			chunk = postfix(&input[in_ind], len);
			//append chunk to output
			for (int i = 0; i <= chunk_len; i++) {
				output[out_ind] = chunk[i];
				out_ind++;
			}
			//push index past parentheses, plus 1 for each parentheses.
			in_ind += 1 + len;
			//Add in everything relevant onto deck and output from within the parentheses
			//while something on deck, order of ops still in favor
			while (deck_ind > 0 && comp(deck[out_ind], input[in_ind]) >= 0) {
				//put top of deck on output
				deck_ind--;
				output[out_ind] = deck[deck_ind];
				out_ind++;
			}
			//printf("\tchunk \"%s\" -> output \"%s\"\n", chunk, output);
			//printf("\tdeck \"%s\"\n", deck);
			//printf("out(%d) in(%d) deck(%d)\n", out_ind, in_ind, deck_ind);
			c = input[in_ind];
			in_ind++;
		}
		else {
			//something on deck
			if (deck_ind > 0) {
				//if current operator on deck has higher-than-or-equal precedence than next operator
				if (comp(deck[deck_ind-1], input[in_ind]) >= 0) {

					//add current char to output
					output[out_ind] = c;
					out_ind++;
					
					//dump deck onto the output
					while (deck_ind > 0 && comp(deck[out_ind], input[in_ind]) >= 0) {
						deck_ind--;
						output[out_ind] = deck[deck_ind];
						out_ind++;
					}
				}
				//next operator has strictly higher precedence
				else {			
					output[out_ind] = c;
					out_ind++;
				}
				
				c = input[in_ind];
				in_ind++;
			}
			//if no operators on deck
			else {
				//add current var into output
				output[out_ind] = c;
				c = input[in_ind];
				in_ind++;
				out_ind++;
			}
		}
	}
	if (deck_ind > 0) {
		printf("should never be called\n");
		for (int i = deck_ind-1; i >= 0; i--) {
			output[out_ind] = deck[i];
			out_ind--;
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
	else
		return NULL;
}

matrix* evaluateEq(char** eq, int len) {

	//Stack on which variables are temporarily stored
	matrix** varstack = calloc(len, sizeof(matrix*));
	
	//Index in the equation being evaluated
	int v_ind = 0;

	//Outputted value
	matrix* outputPtr = malloc(sizeof(matrix));

	bool firstVar = true;

	//Iterate over string.  Algorithm is performed in a single simple pass, so i
	//is only changed by ticking one each loop
	for (int i = 0; i < len; i++) {
		//If the character is an operator
		if (op(eq[i])) {
			//Perform operation on two top values of stack
			matrix* temp = operate(varstack[v_ind-2], varstack[v_ind-1], eq[i]);
			outputPtr = matrix_add(temp, outputPtr);
			
			//Move stack pointer one place down
			v_ind --;
			//Add the resultant onto the top of the stack
			varstack[v_ind-1] = temp;
		}
		//If the character is a variable name
		else {
			matrix* v = variable_get_matrix(eq[i]);
			if (firstVar) {
				matrix_init(outputPtr, v->rows, v->cols);
				firstVar = false;
			}
			//Simply add character to stack
			varstack[v_ind] = v;
			v_ind++;
		}
	}
	//Stack is no longer needed
	free(varstack);
	return outputPtr;
}
