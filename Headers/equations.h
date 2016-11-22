#ifndef EQUATIONS_H
#define EQUATIONS_H

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>


//Establishes order of operations
// By standard: '*' > '+' = '-'
//Returns: value > 0 if c1 has greater precedence than c2
//	   value < 0 if c1 has lower precedence than c2
//	   0 if c1 and c2 have same precedence (- and +)
int comp(char* c1, char* c2);

//Returns: true if c is either +,-, or *, else false
bool op(char* c);

//Returns: true if c is either '(' or ')', else false
bool paren(char* c);

//Converts an array of strings into postfix ordering
char** postfix(char** input, int len);

//Performs the appropriate matrix operation on the two values, using op
//Returns: the resultant matrix pointer
matrix* operate(matrix* val1, matrix* val2, char* op);

//Evaluate array of strings in postfix ordering
//Returns: success of parsing string.  True if its a valid equation
bool evaluateEq(char** eq, int len, matrix* outputPtr);

#endif
