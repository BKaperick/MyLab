#ifndef TESTHEADER_H
#define TESTHEADER_H

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

struct test_str {
	char* data[2];
};

typedef struct test_str test_str;

extern test_str obj;
extern test_str* ptr;

void func();

#endif
