#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>




typedef struct string_array{
	char ** strings;
	int num_strings;
}string_array;

void string_array_print(string_array * array);
string_array * split_string(char * string, char del);

#endif
