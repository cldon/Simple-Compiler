// contains	your main routine and any routines decide	
// you need	to run your program or	parse command line arguments
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "parser.h"

int main(int argc, char **argv) {
	FILE *file;
	file = fopen(argv[1], "r");

	// printf("%s %d\n", argv[1], strlen(argv[1]));
	char output[strlen(argv[1]) + 2];

	strcpy(output, argv[1]);

	// output[strlen(argv[1]) + 2-1] = 'm';
	// output[strlen(argv[1]) + 2-2] = 's';
	// output[strlen(argv[1]) + 2-3] = 'a';
	// output[strlen(argv[1]) + 2-4] = '.';

	// printf("Output: %s\n", output);


	// int len = strlen(argv[1]) - 2;

	startFile(output);

	loadTokens(file);
	
	fclose(file);

	endFile(output);

}