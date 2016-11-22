#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

//parse the command user gives.  Calls any of the other necessary functions
//defined in frontend
bool parse(char* input) {
    printf("parsing\n");	

	if (strcmp(input, "exit\n") == 0)
		return true;

	//Allocates memory to store separate words
	char** words = malloc(5*sizeof(char*));

	//Gets the total length of input and verifies it is not too long
	int len = strlen(input);
	
	//Which index in words the next word is saved
	int wrds_index = 0;

	//The length of the current word
	int wordlength = 0;

	//Pointer to all input with null characters inserted
	//to delimit each word
	char* word = malloc(48*sizeof(char));
	
	//Current inserting index into word
	int w_index = 0;

	
	//left_hand = true;

	for (int i = 0; i < len; i++) {
		//Checks if left hand side of equality is using index notation
		if (input[i] == '[') {
			if (i > 0 && input[i-1] == '$')
				i++;
			else
				input[i] = '$';
		}	
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
			if (input[i] == '\n')
				break;
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
	
    //Running file
	if (args == 2 && strcmp("run", words[0]) == 0) {
		char line[48];
		FILE *fp = fopen(words[1], "r");
		while (fgets(line, sizeof line, fp)) {
			output = true;
			printf("going in...\n");
            parse(line);
		}
		if (!output)
			printf("FILE DOES NOT EXIST ");
	}
	return output;
}



int main() {
    char* input = "run testing_test.myl\n";
    parse(input);  
	return 1;
}
