#include "matrix.h"
#include "frontend.h"
#include "equations.h"

variable_holder vh = {.size = 0, .space = INITIAL_SIZE, .names_ext = NULL, .mats_ext = NULL};
variable_holder* vhp = &vh;

//Characters not allowed in varaible names
char var_char_disallowed[NUM_DISALLOWED] = {'[',']','0','1','2','3','4','5','6','7','8','9'};

//Operators currently defined
char operators[NUM_OPERATORS] = {'+', '-', '*', '=', '(', ')'};

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
	int len = strlen(name);
	int ind1 = 0;
	int ind2 = 0;
	
	//Case where the name is referencing a submatrix
	for (int i = 0; i < len; i++) {
		if (name[i] == '$') {
			name[i] = '\0';
			if (ind1 == 0)
				ind1 = i+1;
			else if (ind2 == 0)
				 ind2 = i+1;
			else
				break;
		}
	}
	int indices[2];
	indices[0] = atoi(&name[ind1]);
	indices[1] = atoi(&name[ind2]);
	matrix* out = malloc(sizeof(matrix));
	out = NULL;
	//Checking if variable name is stored in memory
	for (int i = 0; i < vhp->size; i++) {
		if (strcmp(variable_names()[i], name) == 0) {
			out = variable_mats()[i];
			break;
		}
	}
	if (out == NULL)
		return NULL;
	if (indices[0] != 0) {
		if (indices[1] != 0) {
			return matrix_segment(out, indices[0], indices[0], indices[1], indices[1]);
		}
		else {
			return matrix_segment(out, indices[0], indices[0], 1, out->cols);
		}
	}
	else if (indices[1] != 0) {
		return matrix_segment(out, 1, out->rows, indices[1], indices[1]);
	}
	return out;
}

//Free any memory allocated to vh.
void variable_free() {
	printf("...freeing memory...\n");
	for (int i = 0; i < vhp->size; i++) {
		matrix_free(variable_mats()[i]);
	}
	if (vhp->mats_ext != NULL)
		free(vhp->mats_ext);
	if (vhp->names_ext != NULL)
		free(vhp->names_ext);
}


//////////////////////////////////
// User Input Functions
//////////////////////////////////

//parse the command user gives.  Calls any of the other necessary functions
//defined in frontend
bool parse(char* input) {
    if (strcmp(input, "\n") == 0)
        return true;
    else if (strcmp(input, "exit\n") == 0)
		return true;

	//Allocates memory to store separate words
	char** words = malloc(MAX_WORDS*sizeof(char*));

	//Gets the total length of input and verifies it is not too long
	int len = strlen(input);
	if (len > MAX_INPUT_SIZE) {
		printf("INPUT TOO LONG ");
		return false;
	}
	
	//Which index in words the next word is saved
	int wrds_index = 0;

	//The length of the current word
	int wordlength = 0;

	//Pointer to all input with null characters inserted
	//to delimit each word
	char* word = malloc(MAX_INPUT_SIZE*sizeof(char));
	
	//Current inserting index into word
	int w_index = 0;

	//True only if a semicolon is in the input, suppresses output
	suppress = false;
	
	//left_hand = true;

	for (int i = 0; i < len; i++) {
		//Checks if left hand side of equality is using index notation
		if (input[i] == '[') {
			if (i > 0 && input[i-1] == '$')
				i++;
			else
				input[i] = '$';
		}	
		if (input[i] == ']') {
			input[i] = '$';
		}
		if (input[i] == ','){
			input[i] = '$';
		}
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
				w_index++;
			}
			//Terminate parsing since end of input is reached
            if (input[i] == '\n')
				break;
		}

        //If the character is a so-called "operator"
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
			
			//If it is a two char operator ('==' is only one currently)
			if (input[i+1] == '=') {
				word[w_index] = input[i];
				word[w_index+1] = input[i+1];
				word[w_index+2] = '\0';
				words[wrds_index] = &(word[w_index]);
				wrds_index++;
				w_index += 3;
				i++;
			}
			//one char operator
			else {
				word[w_index] = input[i];
				word[w_index+1] = '\0';
				words[wrds_index] = &(word[w_index]);
				wrds_index++;
				w_index += 2;
			}
		}
		else {
			//Simply take in next letter to word
			wordlength++;
			word[w_index] = input[i];
			w_index++;
		}
	}
	int args = wrds_index;
	bool output = false;

    //A line starting with '#' is a comment and should be ignored
    if (words[0][0] == '#')
        return true;

	//Print out all words for debugging purposes
	for(int i =0; i<args; i++)	
		printf("word %d \"%s\"\n", i, words[i]);
	
	//Instantiating a new matrix
	if (args == 4 && strcmp(DEFINE, words[0]) == 0) {
		output = define(&(words[1]));
	}
	
	//Running file
	else if (args == 2 && strcmp(RUN, words[0]) == 0) {
		char line[MAX_INPUT_SIZE];
		FILE *fp = fopen(words[1], "r");
		while (fgets(line, sizeof line, fp)) {
			output = true;
			printf("going in...\n");
            parse(line);
		}
		if (!output)
			printf("FILE DOES NOT EXIST ");
	}

	//Print matrix
	//Can probably be replaced by simply entering just the matrix name?
	else if (args == 1) {
		output = variable_print(words[0]);	
	}
	else if (strcmp(words[0], PRINT) == 0) {
		if (strcmp(words[1], "all") == 0) {
			variable_printall();
			output = true;
		}
		else
			output = variable_print(words[1]);
	}

	//Store evaluation of RHS into LHS.
	else if (strcmp(words[1], "=") == 0) {
		//Left hand side can only be a valid matrix name or segment
		matrix* lhMatrix = variable_get_matrix(words[0]);
		if (!lhMatrix) {
			printf("LEFT HAND SIDE MUST BE VALID VARIABLE REFERENCE ");
		}
		else if (args >= 3) {
			output = evaluateEq(&(words[2]), args-2, lhMatrix);
			//matrix_print(lhMatrix);
			if (!output) {
				printf("RIGHT HAND SIDE IS INVALID ");
			}
		}
		else {
			printf("CANNOT END AN EXPRESSION WITH '=' ");
		}	
	}
	//
	else if (strcmp(words[1], "==") == 0 || (strcmp(words[1], "=") == 0 && strcmp(words[3], "=") == 0)) {
		matrix* lhMatrix = variable_get_matrix(words[0]);
		matrix* rhMatrix = malloc(sizeof(matrix));

		//Stores the evaluation of RHS into rhMatrix
		output = evaluateEq(&(words[2]), args-2, rhMatrix);
		
		//Set tolerance for equality check
		double tol;
		if (strcmp(words[1], "==") == 0)
			tol = strtof(words[1], NULL);
		else
			tol = DEFAULT_TOL;

		double val = (double)matrix_compare(lhMatrix, rhMatrix, tol);
		matrix* temp = malloc(sizeof(matrix));
		matrix_init(temp,1,1);
		temp->data[0][0] = val;
		variable_add(temp, "temp");
		output = true;
	}

	//Calculate and print result, but don't save it
	else {
		matrix* temp = malloc(sizeof(matrix));
		output = evaluateEq(words, args, temp);
		variable_add(temp, "temp");
		//matrix_print(temp);
	}

	//add in other functionality here
	//free(words);
	//free(word);
	return output;
}

//Prints name if it exists in memory, otherwise prints nothing and
//Returns: true if name exists.
bool variable_print(char* name) {
	matrix* m = variable_get_matrix(name);
	if (m == NULL) {
		printf("MATRIX \"%s\" NOT DEFINED ");
		return false;
	}
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
	if (!only_valid_varchars(name)) {
		printf("\"%s\" IS AN INVALID VARIABLE NAME ");
		return false;
	}
	//Prepares rows and checks if its valid
	uint32_t rows = atoi(input[1]);
	if (rows <= 0) {
		printf("ROW NUMBER IS INVALID ");
		return false;}

	//Prepares columns and checks if its valid
	uint32_t cols = atoi(input[2]);
	if (cols <= 0) {
		printf("COLUMN NUMBER IS INVALID ");
		return false;}

	//Creates new matrix object
	matrix* m = malloc(sizeof(matrix));
	if (m == NULL) {
		printf("MEMORY ALLOCATION FAILED ");
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
			printf("MEMORY ALLOCATION FAILED ");
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
			printf("MEMORY ALLOCATION FAILED ");
			return false;
		}
	}
	variable_mats()[sz] = mat;
	variable_names()[sz] = name;
	vhp->size++;
	if (!suppress) variable_print(name);
	return true;	
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


