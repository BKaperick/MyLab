#include "matrix.h"
#include "frontend.h"



void variable_init(variable_holder* holder) {
	holder->size = 0;	
	holder->space = INITIAL_SIZE;
}

matrix** variable_mats(variable_holder* holder) {
	if (holder->mats_ext == NULL) {
		return holder->mats;
	}
	return (holder->mats_ext);
}

char** variable_names(variable_holder* holder) {
	if (holder->names_ext == NULL) {
		return holder->names;
	}
	return (holder->names_ext);
}	

matrix* variable_get_matrix(variable_holder* holder, char* name) {
	for (int i = 0; i < holder->size; i++) {
		if (holder->names[i] == name) {
			return holder->mats[i];
		}
	}
	return NULL;	
}

bool variable_add(variable_holder* holder, matrix* mat, char* name) {
	int sz = holder->size;
	if (sz < holder->space) {}
	else if (holder->mats_ext == NULL) {
		holder->mats_ext = malloc(2*INITIAL_SIZE*sizeof(matrix));
		holder->names_ext = malloc(2*INITIAL_SIZE*sizeof(char));
		if (!(holder->mats_ext) || !(holder->names_ext) ) {
			return false;
		}
		for (int i = 0; i < sz; i++) {
			holder->mats_ext[i] = holder->mats[i];
			holder->names_ext[i] = holder->names[i];
		}
		holder->space *= 2;
	}
	else {
		holder->mats_ext = realloc(holder->mats_ext, 2*holder->space*sizeof(matrix));	
		holder->names_ext = realloc(holder->names_ext, 2*holder->space*sizeof(char));
		holder->space *= 2;
		if (!(holder->mats_ext) || !(holder->names_ext) ) {
			return false;
		}
	}	
	variable_mats(holder)[sz] = mat;
	variable_names(holder)[sz] = name;
	holder->size++;
	return true;	
}


//parse the command user gives.
//Currently supports:
//	define [matrix name] [rows] [columns]
//	[matrix name] = x1,x2,...,xn;xn+1,...
//	[matrix name] = [matrix name] [+/-/*] [matrix name]  
bool parse(variable_holder* vh, char* input) {
	char* temp = strtok(input, " \t");
	while (temp != NULL) {
		if (strcmp(temp, DEFINE) == 0) {
			bool val = define(vh, input);
			if (!val)
				return false;
			return true;
		}
	}
	return false;

}

bool define(variable_holder* vh, char* input) {
	char* temp = strtok(input, " \t");
	temp = strtok(NULL, " \t");
	if (temp == NULL)
		return false;
	char* name = temp;
	temp = strtok(NULL, " \t");
	if (temp == NULL)
		return false;
	uint32_t rows = atoi(temp);
	if (rows <= 0)	
		return false;
	temp = strtok(NULL, " \t");
	if (temp == NULL)
		return false;
	uint32_t cols = atoi(temp);
	if (cols <= 0)	
		return false;
	matrix* m = malloc(sizeof(matrix));
	if (m == NULL)
		return false;
	matrix_init(m,rows,cols);
	return variable_add(vh, m, name);
}	

