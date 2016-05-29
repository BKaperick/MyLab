#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

//>0 if c1 > c2
int comp(char c1, char c2) {
	if (c2 == '\0')
		return 2;
	if (c1 == '*') {
		if (c2 == '*')	{return 0;}
		else{	return 1;}
	}
	if (c2 == '*') {
		return -1;
	}
	else	{return 0;}	
}
bool op(char c) {
	return (c == '+' || c == '-' || c == '*');
}

bool paren(char c) {
	return (c == '(' || c == ')');
}

char* postfix(char* input, int len) {
	//printf("\tpostfix(\"%s\", %d)\n", input, len);
	char c = input[0];
	char* output = calloc(len, sizeof(char));
	int out_ind = 0;
	int in_ind = 1;
	char* deck = calloc(len, sizeof(char));
	int deck_ind = 0;
	while (c != '\0' && in_ind <= len) {
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
			char* chunk;// = calloc(len - in_ind, sizeof(char));
			int len = 0;
			int chunk_len = 0;
			char exp = input[in_ind];
			while (depth >= 1) {
				if (exp == '(')
					depth++;
				else if (exp == ')')
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
			while (deck_ind > 0 && comp(deck[out_ind], input[in_ind]) >= 0) {
				deck_ind--;
				output[out_ind] = deck[deck_ind];
				deck[deck_ind] = '\0';
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
						deck[deck_ind] = '\0';
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

matrix* operate(char* arg1, char* arg2, char op) {
	
	matrix* val1 = variable_get_matrix(arg1);
	matrix* val2 = variable_get_matrix(arg2);
	if (op == '+')
		return matrix_add(val1, val2);
	else if (op == '-')
		return matrix_sub(val1, val2);
	else if (op == '*')
		return matrix_mult(val1, val2);
}

matrix* evaluateEq(char* eq, int len) {

	//Stack on which variables are temporarily stored
	varstack = calloc(len, sizeof(char));
	
	//Index in the equation being evaluated
	v_ind = 0;

	//Outputted value
	output = 0;

	//Iterate over string.  Algorithm is performed in a single simple pass, so i
	//is only changed by ticking one each loop
	for (int i = 0; i < len; i++) {
		//If the character is an operator
		if (op(eq[i])) {
			//Perform operation on two top values of stack
			double temp = operate(varstack[v_ind-2], varstack[v_ind-1], eq[i]);
			output += temp;
			//Move stack pointer one place down
			v_ind --;
			//Add the resultant onto the top of the stack
			varstack[v_ind-1] = temp;
		}
		//If the character is a variable name
		else {
			//Simply add character to stack
			varstack[v_ind] = eq[i];
			v_ind++;
		}
	}
	//Stack is no longer needed
	free(varstack);
	return output;
}

int main() {
	int len = 19;
	//char input[19] = "a+b-c*d-e-f+g*h*i";
	char* pf;// = postfix(input, len);
	//printf("postfix: \"%s\"\n", pf);
	len = 22;
	char input2[22] = "a+(b-c)+d*(e-(f+g)*h)";
	pf = postfix(input2, len);
	printf("postfix: \"%s\"\n", pf);
	return 1;
}
//ab+c-d*

//ab+cd*-
