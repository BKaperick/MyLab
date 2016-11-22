# MyLab
Basically a clone of MatLab for practice in C and learning some stable algorithms for modern matrix computations.

1. COMPILING

	To compile, run the command
	
	    $gcc -o [executable_name] -std=c99 -Wall main.c matrix_unary.c matrix_binary.c frontend.c equations.c

2. USER INTERFACE


	a. FUNCTIONS
	
		i. define [variable_name] [number of rows] [number of columns]
		
			Stores a new zero matrix of this size in memory.  Then, this matrix can be
			referred to in future computations.
	
		ii. print [variable_name / "all"]

			Prints out the variable name and contents of that matrix.  Note, if you type
			"all" instead of a valid variable name, it will return all variable names 
			and contents stored in memory.

			If just printing one variable (instead of "all"), you can skip the "print" key word.
			i.e., to print element X[i,j], simply type ">X[i,j]".	
		 
		iii. [variable_name] = [variable_name]

			Assignment sets the left hand variable equal to the right hand variable.
			Note: the left hand does not have to be defined already!

		iv. [variable_name / submatrix] = [any algebraic/boolean manipulation of variables or on-the-fly scalars]

			First, the right hand side is evaluated, and then stores this value into the			    
			left hand side.  It will be invalid syntax if the dimensions do not match.
	
			If the left hand side is not explicitly given, the result of the right hand side
			will be stored in a variable "temp".

			Currently, operators implemented are '+', '-', '*', '=='

			Examples of valid syntax are:

			"A = 2*B + 3*(C + D)"
			"5+2*(3-5)"
			"A = 2*A"
			"A == B"
			"C = A == B"
			"A[2][3] = 5 + 3 + C[2,3]"
			"A[2][1] = 5"
			"A [ 2 ][1] =  5"
			"A   [2]   [1]=5"
	
			More sophisticated indexing is also possible. Consider the following:

			A = [1 2 3 4 ]
			    [2 4 6 8 ]
			    [3 6 9 12]
			
			Then, we can refer to segments of it via:

			A[3][2] = A[3,2] = 6
			A[2][]  = A[2]   = [2 4 6 8]
			A[][1]  = A[,1]  = [1]
					   [2]
					   [3]

			Exercise caution for right now, spacing issues are not fully resolved.
			Consider "A[3, 2]", "A[ ][1]", "A[2] [2]", "A [2][2]" and other similar cases
			to be undefined until this section is removed from the README.
						

		vii. run
			
			Keyword used to run text files.  Each line of the file should be a valid mylab
			command.  A syntax error will be returned if the file does not exist, or is
			not properly formatted.  The preferred file ending for mylab files is ".myl",
			but any text file will run.

			If file.myl contains:
			"""
			define X 2 2;
			X = 2*X
			"""
			Then, the command ">run file.myl" initializes a 2x2 matrix X, and prints the 
			reassignment,

			[2.0000 0.0000]
			[0.0000 2.0000]		


		vi. exit
		
			Performs a clean exit of the program.  Frees any allocated memory.

	b. OTHER RESTRICTIONS AND NOTES

		i. Variable names cannot include any of the following:
		
			'[', ']', '0'-'9'
		
		ii. Variable names cannot exactly match any of the following:
			
			"all", "temp"	
		
		iii. Any white space in between variable names, operators, etc. can be any number of
	             spaces without issue.

			In fact, between operators, no space is necessary at all.  For example, all
			of the following are valid:

			"A=B+C"
			"A =B+    C"
			"A=  B +C"
		
		iv.  Output can be supressed in any function by including a semicolon in your input.
		     Note: by default, defining new variables will print the matrix of interest to stdout.
		    
 			A semicolon anywhere in the input will work, for example:
			
			"varA = varB;    -- Good use of semicolon
			"var;A=   varB"  -- I would strongly suggest not doing this.  It looks bad.
			"varA; =varB"	 -- This is also just confusing.  I would not suggest doing it.
