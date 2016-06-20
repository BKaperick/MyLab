#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include "matrix.h"
#include "frontend.h"
#include "equations.h"

int testAlgebra(char* eq, double sol) {
	int output = 0;
	if (parse(eq)) {
		if (variable_get_matrix("temp")->data[0][0] == sol) {
			output++;
		}
	}
	return output;
}

double testAllAlgebra() {
	double output = 0;
	output += testAlgebra("2+3;\n",5.0);
	output += testAlgebra("5 + ;6*(3-2) - 4*3\n", -1);
	output += testAlgebra("2.3 +; 3.2*5 -1*0\n", 17.3);
	parse("run setuptests.myl\n");
	
	return output / 3.0;
}
