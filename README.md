# MyLab
Basically a clone of MatLab for practice in C and learning some stable algorithms for modern matrix computations.

1. COMPILING

To compile, run the command
$gcc -o [executable_name] -std=c99 -Wall main.c matrix_unary.c matrix_binary.c frontend.c

2. USER INTERFACE


	a. FUNCTIONS
	
		i. define [variable_name] [number of rows] [number of columns]
		
			Stores a new zero matrix of this size in memory.  Then, this matrix can be
			referred to in future computations.
	
		ii. print [variable_name / "all"]

			Prints out the variable name and contents of that matrix.  Note, if you type
			"all" instead of a valid variable name, it will return all variable names 
			and contents stored in memory.	
		 
		iii. [variable_name] = [variable_name]

			Assignment sets the left hand variable equal to the right hand variable.
			Note: the left hand does not have to be defined already!

		iv. [variable_name] = [variable_name] [+ / * / -] [variable_name]

			First, the right hand side is evaluated, and then stores this value into the			    
			left hand side.  It will be invalid syntax if the dimensions do not match.

	b. OTHER RESTRICTIONS AND NOTES

		i. Variable names cannot include any of the following:
		
			'[', ']', '0'-'9'
		
		ii. Variable names cannot exactly match any of the following:
			
			"all"	
		
		iii. Any white space in between variable names, operators, etc. can be any number of
	             spaces without issue.

			  In fact, between operators, no space is necessary at all.  For example, all
			  of the following are valid:

			  "A=B+C"
			  "A =B+    C"
			  "A=  B +C"

