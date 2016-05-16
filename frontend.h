#ifndef FRONTEND_H
#define FRONTEND_H

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

#define INITIAL_SIZE 10
#define DEFINE "define"
struct variable_holder {
	matrix* mats[INITIAL_SIZE];
	char* names[INITIAL_SIZE];
	matrix** mats_ext;
	char** names_ext;
	int size;
	int space;
};

typedef struct variable_holder variable_holder;

//variable_holder vh = {.size = 0, .space = INITIAL_SIZE};
//variable_holder* vhp;

void variable_init(variable_holder*);

bool variable_add(variable_holder* holder, matrix* mat, char* name);

matrix* variable_get_matrix(variable_holder* holder, char* name);

bool define(variable_holder* vh, char* input);

//parse the command user gives.
//Currently supports:
//	define [matrix name] [rows] [columns]
//	[matrix name] = x1,x2,...,xn;xn+1,...
//	[matrix name] = [matrix name] [+/-/*] [matrix name]  
bool parse(variable_holder* vh, char* input);


#endif
