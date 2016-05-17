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

void variable_printall();

matrix** variable_mats();

char** variable_names();

bool variable_add(matrix* mat, char* name);

matrix* variable_get_matrix(char* name);

bool define(char** input);

//parse the command user gives.
//Currently supports:
//	define [matrix name] [rows] [columns]
//	[matrix name] = x1,x2,...,xn;xn+1,...
//	[matrix name] = [matrix name] [+/-/*] [matrix name]  
bool parse(char* input);

bool variable_print(char* name);
#endif
