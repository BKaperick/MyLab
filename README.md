# MyLab
Basically a clone of MatLab for practice in C and learning some stable algorithms for modern matrix computations.

1. COMPILING

To compile, run the command
$gcc -o [executable] -std=c99 -Wall main.c matrix_unary.c matrix_binary.c frontend.c

2. USER INTERFACE

	a. FUNCTIONS
	
		i. define [variable_name] [number of rows] [number of columns]
		
			Stores a new zero matrix of this size in memory.  Then, this matrix can be
			referred to in future computations.
	
		ii. print [variable_name / "all"]

			Prints out the variable name and contents of that matrix.  Note, if you type 			    "all" instead of a valid variable name, it will return all variable names 
			and contents stored in memory.	
