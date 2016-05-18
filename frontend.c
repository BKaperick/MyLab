#include "matrix.h"
#include "frontend.h"

variable_holder vh = {.size = 0, .space = INITIAL_SIZE, .names_ext = NULL, .mats_ext = NULL};
variable_holder* vhp = &vh;

//Characters not allowed in varaible names
char var_char_disallowed[NUM_DISALLOWED] = {'[',']','0','1','2','3','4','5','6','7','8','9'};

//Operators currently defined
char operators[NUM_OPERATORS] = {'+', '-', '*', '='};

bool suppress;

///////////////////////////////////
// Variable Holder helper functions
///////////////////////////////////

//Returns pointer to matrices in memory
matrix** variable_mats() {
	if (vhp->mats_ext == NULL) {
		return vhp->mats;
	}
	return (vhp->mats_ext);
}

//Returns pointer to variable names in memory
char** variable_names() {
	if (vhp->names_ext == NULL) {
		return vhp->names;
	}
	return (vhp->names_ext);
}

//Retrieves the matrix corresponding to a given name in memory.
//Returns: pointer to that matrix, or NULL if it doesn't exist.
matrix* variable_get_matrix(char* name) {
	for (int i = 0; i < vhp->size; i++) {
		if (strcmp(variable_names()[i], name) == 0) {
			return variable_mats()[i];
		}
	}
	return NULL;	
}


//////////////////////////////////
// User Input Functions
//////////////////////////////////

//parse the command user gives.  Calls any of the other necessary functions
//defined in frontend
bool parse(char* input) {
	
	if (strcmp(input, "exit\n") == 0)
		return true;

	//Allocates memory to store separate words
	char** words = malloc(MAX_WORDS*sizeof(char*));

	//Gets the total length of input and verifies it is not too long
	int len = strlen(input);
	if (len > MAX_INPUT_SIZE) {
		printf("INPUT TOO LONG");
		return false;
	}
	
	//Which index in words the next word is saved
	int wrds_index = 0;

	//The length of the current word
	int wordlength = 0;

	//Pointer to all input with null characters inserted
	//to delimit each word
	char* word = malloc(MAX_INPUT_SIZE*sizeof(char));
	
	//Check necessary for branching logic
	bool midword = false;

	//Current inserting index into word
	int w_index = 0;

	//True only if a semicolon is in the input, suppresses output
	suppress = false;

	for (int i = 0; i < len; i++) {
		//Suppresses output and skips over this character
		if (input[i] == ';')
			suppress = true;

		//Signifies new word (' ') or end of input ('\n')
		else if (input[i] == ' ' || input[i] == '\n') {
			//If a word is currently being processed (this ' ' or '\n' comes right after a word)
			if (wordlength > 0) {
				//Add word to words array
				word[w_index] = '\0';
				words[wrds_index] = &(word[w_index-wordlength]);

				//Update indicator variables
				wrds_index++;
				wordlength = 0;
				midword = false;
				w_index++;
			}
			if (input[i] == '\n')
				break;
		}
		else if (in_operators(input[i])) {
			if (wordlength > 0) {
				//Save current word
				word[w_index] = '\0';
				words[wrds_index] = &(word[w_index-wordlength]);
				wrds_index++;
				wordlength = 0;
				w_index++;
			}
			
			//Also save operator as a separate word
			word[w_index] = input[i];
			word[w_index+1] = '\0';
			words[wrds_index] = &(word[w_index]);
			wrds_index++;
			midword = false;
			w_index += 2;
		}
		else {
			midword = true;
			wordlength++;
			word[w_index] = input[i];
			w_index++;
		}
	}
	int args = wrds_index;
	for (int i = 0; i < args; i++)
		printf("word \"%s\"\n", words[i]);
	if (args == 4 && strcmp(DEFINE, words[0]) == 0) {
		return define(&(words[1]));
	}
	else if (args == 2 && strcmp(words[0], PRINT) == 0) {
		if (strcmp(words[1], "all") == 0) {
			variable_printall();
			return true;
		}
		return variable_print(words[1]);
	}
	else if (args >= 3 && strcmp(words[1], "=") == 0) {
		//matrix* lhand = variable_get_matrix(words[0]);
		if (only_valid_varchars(words[0])) {
			if (args == 3) {
				
				return variable_add(variable_get_matrix(words[2]), words[0]);
			}
			else if (args == 5) {
				return (bool) variable_evaluate(words[0], words[2], words[3], words[4]);
			}
		}

	}
		//add in other functionality here
	return false;
}

//Prints name if it exists in memory, otherwise prints nothing and
//Returns: true if name exists.
bool variable_print(char* name) {
	matrix* m = variable_get_matrix(name);
	if (m == NULL)
		return false;
	if (!suppress) matrix_print(m);
	return true;
}

//Prints every variable in memory
void variable_printall() {
	if (!suppress) {
		for (int i = 0; i < vhp->size; i++) {
			if (i > 0) printf("\n");
			printf("\"%s\":\n", variable_names()[i]);
			matrix_print(variable_mats()[i]);
		}
	}
}

//Defines a new variable in memory
//Returns: success of operation
bool define(char** input) {

	//Prepares name and checks if its valid
	char* name = input[0];
	if (!only_valid_varchars(name))
		return false;
	
	//Prepares rows and checks if its valid
	uint32_t rows = atoi(input[1]);
	if (rows <= 0) {
		return false;}

	//Prepares columns and checks if its valid
	uint32_t cols = atoi(input[2]);
	if (cols <= 0) {
		return false;}

	//Creates new matrix object
	matrix* m = malloc(sizeof(matrix));
	if (m == NULL) {
		return false;}
	matrix_init(m,rows,cols);
	
	//Adds new object to memory under name
	return variable_add(m, name);
}	

//Helper function called by "define".  Actually adds matrix to memory
//Returns: success of operation
bool variable_add(matrix* mat, char* name) {

	//define local variable since it's referenced a lot
	int sz = vhp->size;
	
	//Overwriting already defined variable.
	for (int i = 0; i < sz; i++) {
		if (strcmp(variable_names()[i], name) == 0) {
			variable_mats()[i] = mat;
			return true;
		}
	}

	//Don't allocate or reallocate memory if size < space
	if (sz < vhp->space) {}
	else if (vhp->mats_ext == NULL) {
		vhp->mats_ext = malloc(4 + INITIAL_SIZE*sizeof(matrix));
		vhp->names_ext = malloc(4 + INITIAL_SIZE*sizeof(char));
		if (!(vhp->mats_ext) || !(vhp->names_ext) ) {
			return false;
		}
		for (int i = 0; i < sz; i++) {
			vhp->mats_ext[i] = vhp->mats[i];
			vhp->names_ext[i] = vhp->names[i];
		}
		vhp->space += 4;
	}
	//Expand extra memory if necessary
	else {
		vhp->mats_ext = realloc(vhp->mats_ext, 4 + vhp->space*sizeof(matrix));	
		vhp->names_ext = realloc(vhp->names_ext, 4 + vhp->space*sizeof(char));
		vhp->space += 4;
		if (!(vhp->mats_ext) || !(vhp->names_ext) ) {
			return false;
		}
	}	
	variable_mats()[sz] = mat;
	variable_names()[sz] = name;
	vhp->size++;
	if (!suppress) variable_print(name);
	return true;	
}

//Retrieves the matrices corresponding to n1 and n2
//Then performs op1 on those matrices
//then stores the result in newname
//returns the result for posterity.
matrix* variable_evaluate(char* newname, char* n1, char* op, char* n2) {
	printf("in eval\n");
	matrix* m1 = variable_get_matrix(n1);
	matrix* m2 = variable_get_matrix(n2);
	if (m1 == NULL || m2 == NULL)
		return NULL;
	matrix* res = malloc(sizeof(matrix));
	if (strcmp(op, "+") == 0) {
		printf("adding\n");
		res = matrix_add(m1, m2);
	}
	else if (strcmp(op, "*") == 0) {
		res = matrix_mult(m1, m2);
	}
	else if (strcmp(op, "-") == 0) {
		res = matrix_sub(m1, m2);
	}
	bool ret = variable_add(res, newname);
	if (!ret) {
		return NULL;
	}
	return res;
}		


//////////////////////////////////
// Helper / Check Functions
//////////////////////////////////

//Checks if op is in operators
//Returns: is op a valid operator?
bool in_operators(char op) {
	for (int i = 0; i < NUM_OPERATORS; i++) {
		if (op == operators[i])
			return true;
	}
	return false;
}

//Checks if a variable name uses only valid characters
//Returns: false if any invalid characters found in name
bool only_valid_varchars(char* name) {
	for (int i = 0; i < strlen(name); i++) {
		for (int j = 0; j < NUM_DISALLOWED; j++) {
			if (name[i] == var_char_disallowed[j])
				return false;
		}
	}
	return true;
}


