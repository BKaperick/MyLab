#include "matrix.h"
#include "frontend.h"

variable_holder vh = {.size = 0, .space = INITIAL_SIZE};
variable_holder* vhp = &vh;


matrix** variable_mats() {
	if (vhp->mats_ext == NULL) {
		return vhp->mats;
	}
	return (vhp->mats_ext);
}

char** variable_names() {
	if (vhp->names_ext == NULL) {
		return vhp->names;
	}
	return (vhp->names_ext);
}	

matrix* variable_get_matrix(char* name) {
	printf("vhp size: %d\n", vhp->size);
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
	variable_mats(vhp)[sz] = mat;
	variable_names(vhp)[sz] = name;
	printf("incrementing size from %d ", vhp->size);
	vhp->size++;
	printf("to %d\n", vhp->size);
	return true;	
}

bool variable_print(char* name) {
	printf("gonna print\n");
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
	char* tempin = malloc(strlen(input)*sizeof(char));
	strcpy(tempin, input);
	char* temp = strtok(tempin, " \t\n");
	if (temp != NULL) {
		if (strcmp(temp, DEFINE) == 0) {
			printf("DEFINITION\n");
			bool val = define(input);
			return val;
		}
		else if (strcmp(temp, PRINT) == 0) {
			//temp = strtok(NULL, " \t\n");
			bool val = variable_print(temp);
			return val;
		}
		//add in other functionality here
	}
	else {
		printf("nothing to be read in input\n");
		return false;
	}
	return false;
}

bool define(char* input) {
	printf("input: %s\n", input);
	char* temp = strtok(input, " \t\n");
	printf("temp: %s\n", temp);
	temp = strtok(NULL, " \t\n");
	if (temp == NULL){printf("check1\n");
		return false;}
	char* name = temp;
	temp = strtok(NULL, " \t\n");
	if (temp == NULL) {printf("check2\n");
		return false;}
	uint32_t rows = atoi(temp);
	if (rows <= 0) {printf("check3\n");
		return false;}
	temp = strtok(NULL, " \t\n");
	if (temp == NULL) {printf("check4\n");
		return false;}
	uint32_t cols = atoi(temp);
	if (cols <= 0) {printf("check5\n");
		return false;}
	matrix* m = malloc(sizeof(matrix));
	if (m == NULL) {printf("check6\n");
		return false;}
	matrix_init(m,rows,cols);
	printf("ready to add\n");
	return variable_add(m, name);
}	

