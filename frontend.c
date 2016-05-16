#include "matrix.h"
#include "frontend.h"

variable_holder vh = {.size = 0, .space = INITIAL_SIZE};
variable_holder* vhp = &vh;


matrix** variable_mats() {
	if (vhp->mats_ext == NULL) {
		printf("still using default storage\n");
		return vhp->mats;
	}
	return (vhp->mats_ext);
}

char** variable_names() {
	if (vhp->names_ext == NULL) {
		printf("still using default storage\n");
		return vhp->names;
	}
	return (vhp->names_ext);
}	

matrix* variable_get_matrix(char* name) {
	printf("vhp size: %d\n", vhp->size);
	matrix_print(vhp->mats[0]);
	for (int i = 0; i < vhp->size; i++) {
		printf("name \"%s\"\n", vhp->names[i]);
		if (vhp->names[i] == name) {
			return vhp->mats[i];
		}
	}
	return NULL;	
}

bool variable_add(matrix* mat, char* name) {
	printf("name: %s\n", name);
	printf("vhp size: %d\n", vhp->size);
	//define local variable since it's referenced a lot
	int sz = vhp->size;
	
	printf("sz = %d\n", sz);

	//Don't allocate or reallocate memory if size < space
	if (sz < vhp->space) {printf("no need to [re]allocate mem\n");}
	else if (vhp->mats_ext == NULL) {
		vhp->mats_ext = malloc(2*INITIAL_SIZE*sizeof(matrix));
		vhp->names_ext = malloc(2*INITIAL_SIZE*sizeof(char));
		if (!(vhp->mats_ext) || !(vhp->names_ext) ) {
			return false;
		}
		for (int i = 0; i < sz; i++) {
			vhp->mats_ext[i] = vhp->mats[i];
			vhp->names_ext[i] = vhp->names[i];
		}
		vhp->space *= 2;
	}
	else {
		vhp->mats_ext = realloc(vhp->mats_ext, 2*vhp->space*sizeof(matrix));	
		vhp->names_ext = realloc(vhp->names_ext, 2*vhp->space*sizeof(char));
		vhp->space *= 2;
		if (!(vhp->mats_ext) || !(vhp->names_ext) ) {
			return false;
		}
	}	
	variable_mats()[sz] = mat;
	printf("adding \"%s\"\n", name);
	vhp->names[sz] = name;
	//variable_names()[sz] = name;
	printf("incrementing size from %d ", vhp->size);
	vhp->size++;
	printf("to %d\n", vhp->size);
	return true;	
}

bool variable_print(char* name) {
	printf("gonna print\"%s\"\n", name);
	matrix* m = variable_get_matrix(name);
	if (m == NULL)
		return false;
	matrix_print(m);
	return true;
}

//parse the command user gives.
//Currently supports:
//	define [matrix name] [rows] [columns]
//	[matrix name] = x1,x2,...,xn;xn+1,...
//	[matrix name] = [matrix name] [+/-/*] [matrix name]  
bool parse(char* input) {
	char* words[MAX_WORDS];
	int len = strlen(input);
	int placement = 0;
	int wordlength = 0;
	char word[MAX_WORD_SIZE];
	for (int i = 0; i < len; i++) {
		if (input[i] == ' ' || input[i] == '\n') {
			word[i] = '\0';
			words[placement] = &(word[i-wordlength]);
			placement++;
			wordlength = 0;
		}
		else if (input[i] == '\0')
			break;
		else {
			wordlength++;
			word[i] = input[i];
		}
	}
	int args = placement;
	
	if (args == 4 && strcmp(DEFINE, words[0]) == 0) {
		printf("DEFINITION\n");
		bool val = define(&(words[1]));
		return val;
	}
	else if (args == 2 && strcmp(words[0], PRINT) == 0) {
		//temp = strtok(NULL, " \t\n");
		bool val = variable_print(words[1]);
		return val;
	}
		//add in other functionality here
	else {
		printf("nothing to be read in input\n");
		return false;
	}
	return false;
}

bool define(char** input) {
	char* name = input[0];
	uint32_t rows = atoi(input[1]);
	if (rows <= 0) {printf("check3\n");
		return false;}
	uint32_t cols = atoi(input[2]);
	if (cols <= 0) {printf("check5\n");
		return false;}
	matrix* m = malloc(sizeof(matrix));
	if (m == NULL) {printf("check6\n");
		return false;}
	matrix_init(m,rows,cols);
	printf("ready to add\n");
	return variable_add(m, name);
}	

