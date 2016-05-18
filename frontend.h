#ifndef FRONTEND_H
#define FRONTEND_H

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

#define INITIAL_SIZE 10
#define DEFINE "define"
#define PRINT "print"
#define INPUT_SIZE 50
#define MAX_WORDS 5
#define MAX_WORD_SIZE 15

#define NUM_DISALLOWED 12
#define NUM_OPERATORS 4

//extern char* names[INITIAL_SIZE];

struct variable_holder {
	matrix* mats[INITIAL_SIZE];
	char* names[INITIAL_SIZE];
	matrix** mats_ext;
	char** names_ext;
	int size;
	int space;
};

typedef struct variable_holder variable_holder;

extern variable_holder vh;
extern variable_holder* vhp;

extern char var_char_disallowed[NUM_DISALLOWED];
extern char operators[NUM_OPERATORS];


void variable_printall();

bool in_operators(char op);

matrix** variable_mats();

char** variable_names();

bool variable_add(matrix* mat, char* name);

matrix* variable_get_matrix(char* name);

//Retrieves the matrices corresponding to n1 and n2
//Then performs op1 on those matrices
//then stores the result in newname
//returns the result for posterity.
matrix* variable_evaluate(char* newname, char* n1, char* op1, char* n2);

bool only_valid_varchars(char* name);

bool define(char** input);

//parse the command user gives.
//Currently supports:
//	define [matrix name] [rows] [columns]
//	[matrix name] = x1,x2,...,xn;xn+1,...
//	[matrix name] = [matrix name] [+/-/*] [matrix name]  
bool parse(char* input);

bool variable_remap(char* n1, char* n2);

bool variable_print(char* name);
#endif
