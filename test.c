#include <stdio.h>
#include <stdlib.h>

int main() {
	char* str0 = "wemust";
	char* str1 = "test";
	char* str2 = "more";
	char* str3 = "ovaltine";
	char* str4 = "plz";

	int* intPtr = malloc(sizeof(int));
	&intPtr = 5;

	int copy = &intPtr;

	&intPtr++;
	printf("%d\n", copy);

	
	return 1;
}
