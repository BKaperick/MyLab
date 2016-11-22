#ifndef FRONTEND_H
#define FRONTEND_H

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

//Initial memory allocated for variables
#define INITIAL_SIZE 10

//Command to define a new variable
#define DEFINE "define"

//Command to run a file
#define RUN "run"

//Command to print a variable
#define PRINT "print"

//Maximum number of words in a command
#define MAX_WORDS 5

//Maximum length of input
#define MAX_INPUT_SIZE 100

//Number of disallowed variable characters
#define NUM_DISALLOWED 12

//Number of operators currently defined
#define NUM_OPERATORS 6

//Default tolerance for double equality checks
#define DEFAULT_TOL .0001

struct variable_holder {
	//Default containers for matrices and variable names
	matrix* mats[INITIAL_SIZE];
	char* names[INITIAL_SIZE];

	//Expandable containers for matrices and variable names if
	//the size exceeds INITIAL_SIZE
	matrix** mats_ext;
	char** names_ext;

	//Number of variables saved
	int size;
	
	//Amount of total space allotted for variables
	//(default is INITIAL_SIZE)
	int space;
};

typedef struct variable_holder variable_holder;

//Running list of all characters that are not allowed to be used in
//variable names
extern char var_char_disallowed[NUM_DISALLOWED];

//Running list of special operator characters
extern char operators[NUM_OPERATORS];

//Master Variable Holder, stores all variables currently in memory
extern variable_holder vh;
extern variable_holder* vhp;


///////////////////////////////////
// Variable Holder helper functions
///////////////////////////////////

//Returns pointer to matrices in memory
matrix** variable_mats();

//Returns pointer to variable names in memory
char** variable_names();

//Retrieves the matrix corresponding to a given name in memory.
//Returns: pointer to that matrix, or NULL if it doesn't exist.
matrix* variable_get_matrix(char* name);

//Deallocate any memory allocated to vh.
void variable_free();


//////////////////////////////////
// User Input Functions
//////////////////////////////////

//parse the command user gives.  Calls any of the other necessary functions
//defined in frontend
bool parse(char* input);

//Prints name if it exists in memory, otherwise prints nothing and
//Returns: true if name exists.
bool variable_print(char* name);

//Prints every variable in memory
void variable_printall();

//Defines a new variable in memory
//Returns: success of operation
bool define(char** input);

//Helper function called by "define".  Actually adds matrix to memory
//Returns: success of operation
bool variable_add(matrix* mat, char* name);


//////////////////////////////////
// Helper / Check Functions
//////////////////////////////////

//Checks if op is in operators
//Returns: is op a valid operator?
bool in_operators(char op);

//Checks if a variable name uses only valid characters
//Returns: false if any invalid characters found in name
bool only_valid_varchars(char* name);



#endif
