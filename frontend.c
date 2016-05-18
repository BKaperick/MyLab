#include "matrix.h"
#include "frontend.h"

variable_holder vh = {.size = 0, .space = INITIAL_SIZE, .names_ext = NULL, .mats_ext = NULL};
variable_holder* vhp = &vh;

char var_char_disallowed[NUM_DISALLOWED] = {'[',']','0','1','2','3','4','5','6','7','8','9'};
char operators[NUM_OPERATORS] = {'+', '-', '*', '='};

matrix** variable_mats() {
	if (vhp->mats_ext == NULL) {
		return vhp->mats;
	}
	return (vhp->mats_ext);
}

bool variable_print(char* name) {
	matrix* m = variable_get_matrix(name);
	if (m == NULL)
		return false;
	matrix_print(m);
	return true;
}

void variable_printall() {
	printf("variables(%d)----------[\n", vhp->size);
	for (int i = 0; i < vhp->size; i++) {
		printf("\"%s\":\n", variable_names()[i]);
		matrix_print(variable_mats()[i]);
	}
	printf("]-------------variables\n");
}

char** variable_names() {
	if (vhp->names_ext == NULL) {
		return vhp->names;
	}
	return (vhp->names_ext);
}

matrix* variable_get_matrix(char* name) {
	for (int i = 0; i < vhp->size; i++) {
		if (strcmp(variable_names()[i], name) == 0) {
			return variable_mats()[i];
		}
	}
	return NULL;	
}

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
	
	return true;	
}

bool in_operators(char op) {
	for (int i = 0; i < NUM_OPERATORS; i++) {
		if (op == operators[i])
			return true;
	}
	return false;
}

bool parse(char* input) {
	char** words = malloc(MAX_WORDS*sizeof(char*));
	int len = strlen(input);
	int placement = 0;
	int wordlength = 0;
	char* word = malloc(MAX_WORD_SIZE*sizeof(char));
	bool midword = false;
	int save_at = 0;
	for (int i = 0; i < len; i++) {
		if (input[i] == ' ' || input[i] == '\n') {
			if (midword) {
				word[save_at] = '\0';
				words[placement] = &(word[save_at-wordlength]);
				placement++;
				wordlength = 0;
				midword = false;
				save_at++;
			}
			if (input[i] == '\n')
				break;
		}
		else if (in_operators(input[i])) {
			if (midword) {
				//Save current word
				word[save_at] = '\0';
				words[placement] = &(word[save_at-wordlength]);
				placement++;
				wordlength = 0;
				i++;
				save_at++;
			}
			
			//Also save operator as a separate word
			if (!midword)
				word[save_at] = input[i];
			else
				word[save_at] = input[i-1];
			word[save_at+1] = '\0';
			words[placement] = &(word[save_at]);
			placement++;
			if (midword)
				i--;
			midword = false;
			save_at += 2;
		}
		else if (input[i] == '\0')
			break;
		else {
			midword = true;
			wordlength++;
			word[save_at] = input[i];
			save_at++;
		}
	}
	for (int i = 0; i < placement; i++) {
		printf("word: \"%s\"\n", words[i]);
	}
	int args = placement;
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
				return variable_remap(words[0], words[2]);
			}
			else if (args == 5) {
				return (bool) variable_evaluate(words[0], words[2], words[3], words[4]);
			}
		}

	}
		//add in other functionality here
	return false;
}

bool only_valid_varchars(char* name) {
	for (int i = 0; i < strlen(name); i++) {
		for (int j = 0; j < NUM_DISALLOWED; j++) {
			if (name[i] == var_char_disallowed[j])
				return false;
		}
	}
	return true;
}

bool define(char** input) {
	char* name = input[0];
	if (!only_valid_varchars(name))
		return false;
	uint32_t rows = atoi(input[1]);
	if (rows <= 0) {
		return false;}
	uint32_t cols = atoi(input[2]);
	if (cols <= 0) {
		return false;}
	matrix* m = malloc(sizeof(matrix));
	if (m == NULL) {
		return false;}
	matrix_init(m,rows,cols);
	return variable_add(m, name);
}	

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

bool variable_remap(char* name1, char* name2) {
	return variable_add(variable_get_matrix(name2), name1);
}
