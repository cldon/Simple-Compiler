// contains the routines you need for opening and loading the object files

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "parser.h"

FILE *f;
int bnum = 0;
int bnumDec = 0;
int fnum = 0; 
char fbname[250]; 
char ffname[250];
char fname[250];
if_state *top_if;
int ifnum = 0;

void startFile(char name[]) {
	char wf[250];
	strcpy(wf, name);
	wf[strlen(name) + 2-1] = 'm';
	wf[strlen(name) + 2-2] = 's';
	wf[strlen(name) + 2-3] = 'a';

	f = fopen(wf, "w");
	// fprintf(f, "\t.CODE\n\t.FALIGN\n\n");
	// fprintf(f, "\tCONST R6, #255\n\tHICONST R6, x7F\n\tHICONST R7, x0\n\n");
	strcpy(fbname, name);
	strcpy(ffname, name);
	fbname[strlen(name)] = 'b';
	ffname[strlen(name)] = 'f';

	strcpy(fname, name);
	fname[strlen(name) + 2-1] = '\0';
	fname[strlen(name) + 2-2] = '\0';
	fname[strlen(name) + 2-3] = '\0';
	fname[strlen(name) + 2-4] = '\0';

	fbname[strlen(name) + 2-1] = '\0';
	fbname[strlen(name) + 2-2] = '\0';
	fbname[strlen(name) + 2-3] = '\0';
	fbname[strlen(name) + 2-4] = 'b';

	ffname[strlen(name) + 2-1] = '\0';
	ffname[strlen(name) + 2-2] = '\0';
	ffname[strlen(name) + 2-3] = '\0';
	ffname[strlen(name) + 2-4] = 'b';
	top_if = NULL;
	
}

void endFile() {
	fclose(f);
}

void jumpMethod(token *tok) {
	printf("%s\n", tok->str);
	fprintf(f, "\tJSR %s\n\n", tok->str);
	free(tok);
}


void writeToken(token *tok) {
	if (tok->type == LITERAL) {
		fprintf(f, "\t;; %d\n", tok->literal_value);
		if (tok->literal_value >= 255) {
			short int num;
			num = tok->literal_value << 8;
			num = num >> 8;
			fprintf(f, "CONST R1 %i\n", num);
			num = tok->literal_value >> 8;
			fprintf(f, "HICONST R1 %i\n", num);
			// printf("NUMBER: %i", i);
		}else if (tok->literal_value < -265) {
			unsigned short int num;
			num = tok->literal_value << 8;
			num = num >> 8;
			fprintf(f, "CONST R1 %i\n", num);
			num = tok->literal_value >> 8;
			num = num & 0b11111111;
			fprintf(f, "HICONST R1 %i\n", (signed)num);
		}
			else {
			fprintf(f, "\tCONST R1, #%d\n", tok->literal_value);
		}

		fprintf(f, "\tSTR R1, R6, #-1\n"
			"\tADD, R6, R6, #-1\n\n"); 
	} else if (tok->type == PLUS) {
		fprintf(f, "\t;; +\n");
		fprintf(f, 
			"\tLDR R0, R6, #0\n"
			"\tLDR R1, R6, #1\n"
			"\tADD R6, R6, #2\n"
			"\tADD R0, R0, R1\n"
			"\tSTR R0, R6, #-1\n"
			"\tADD R6, R6, #-1\n\n");
	} else if (tok->type == MINUS) {
		fprintf(f, "\t;; -\n");
		fprintf(f, 
			"\tLDR R0, R6, #0\n"
			"\tLDR R1, R6, #1\n"
			"\tADD R6, R6, #2\n"
			"\tSUB R0, R0, R1\n"
			"\tSTR R0, R6, #-1\n"
			"\tADD R6, R6, #-1\n\n");
	} else if (tok->type == MUL) {
		fprintf(f, "\t;; *\n");
		fprintf(f, 
			"\tLDR R0, R6, #0\n"
			"\tLDR R1, R6, #1\n"
			"\tADD R6, R6, #2\n"
			"\tMUL R0, R0, R1\n"
			"\tSTR R0, R6, #-1\n"
			"\tADD R6, R6, #-1\n\n");
	} else if (tok->type == DIV) {
		fprintf(f, "\t;; /\n");
		fprintf(f, 
			"\tLDR R0, R6, #0\n"
			"\tLDR R1, R6, #1\n"
			"\tADD R6, R6, #2\n"
			"\tDIV R0, R0, R1\n"
			"\tSTR R0, R6, #-1\n"
			"\tADD R6, R6, #-1\n\n");
	} else if (tok->type == MOD) {
		fprintf(f, "\t;; %%\n");
		fprintf(f, 
			"\tLDR R0, R6, #0\n"
			"\tLDR R1, R6, #1\n"
			"\tADD R6, R6, #2\n"
			"\tMOD R0, R0, R1\n"
			"\tSTR R0, R6, #-1\n"
			"\tADD R6, R6, #-1\n\n");

	} else if (tok->type == AND) {
		fprintf(f, "\t;; AND\n");
		fprintf(f, 
			"\tLDR R0, R6, #0\n"
			"\tLDR R1, R6, #1\n"
			"\tADD R6, R6, #2\n"
			"\tAND R0, R0, R1\n"
			"\tSTR R0, R6, #-1\n"
			"\tADD R6, R6, #-1\n\n");
	} else if (tok->type == OR) {
		fprintf(f, "\t;; OR\n");
		fprintf(f, 
			"\tLDR R0, R6, #0\n"
			"\tLDR R1, R6, #1\n"
			"\tADD R6, R6, #2\n"
			"\tOR R0, R0, R1\n"
			"\tSTR R0, R6, #-1\n"
			"\tADD R6, R6, #-1\n\n");
	} else if (tok->type == DROP) {
		fprintf(f, "\t;; DROP\n");
		fprintf(f, 
			"\tADD R6, R6, #1\n\n");
	} else if (tok->type == DUP) {
		fprintf(f, "\t;; DUP\n");
		fprintf(f, 
			"\tLDR R0, R6, #0\n"
			"\tSTR R0, R6, #-1\n"
			"\tADD R6, R6, #-1\n");
	} else if (tok->type == SWAP) {
		fprintf(f, "\t;; SWAP\n");
		fprintf(f, 
			"\tLDR R0, R6, #0\n"
			"\tLDR R1, R6, #1\n"
			// "\tADD R6, R6, #2\n"
			"\tSTR R0, R6, #1\n"
			"\tSTR R1, R6, #0\n");
			// "\tADD R6, R6, #-2\n\n");
	} else if (tok->type == ROT) {
		fprintf(f, "\t;; ROT\n");
		fprintf(f, 
			"\tLDR R0, R6, #0\n"
			"\tLDR R1, R6, #1\n"
			"\tLDR R2, R6, #2\n"
			// "\tADD R6, R6, #3\n"
			"\tSTR R1, R6, #2\n"
			"\tSTR R0, R6, #1\n"
			"\tSTR R2, R6, #0\n");
			// "\tADD R6, R6, #-3\n\n");
	} else if (tok->type == IDENT) {
		fprintf(f, "\t.CODE\n\t.FALIGN\n\n");
		fprintf(f, "\t;; function\n%s\n", tok->str);
		fprintf(f, 
			"\tADD R6, R6, #-3\n"
			"\tSTR R7, R6, #1\n"
			"\tSTR R5, R6, #0\n"
			"\tADD R5, R6, #0\n\n");
	} else if (tok->type == LE) {
		fprintf(f, "\t;; LE\n");
		fprintf(f, 
			"\tLDR R0, R6, 0\n"
			"\tLDR R1, R6, 1\n"
			"\tADD R6, R6, #2\n"
			"\tCMP R0, R1\n"
			"\tBRp %s%i\n" 
			"\tCONST R0, #1\n"
			"\tSTR R0, R6, #-1\n"
			"\tJMP %s%i\n"
			"%s%i\n"
			"\tCONST R0, #0\n"
			"\tSTR R0, R6, #-1\n"
			"%s%i\n"
			"\tADD R6, R6, #-1\n\n", fbname, bnum, fbname, bnum + 1, fbname, bnum, fbname, bnum + 1);
		bnum += 3;
	} else if (tok->type == LT) {
		// printf("INSIDE LE WRITER");
		fprintf(f, "\t;; LT\n");
		fprintf(f, 
			"\tLDR R0, R6, 0\n"
			"\tLDR R1, R6, 1\n"
			"\tADD R6, R6, #2\n"
			"\tCMP R0, R1\n"
			"\tBRzp %s%i\n" 
			"\tCONST R0, #1\n"
			"\tSTR R0, R6, #-1\n"
			"\tJMP %s%i\n"
			"%s%i\n"
			"\tCONST R0, #0\n"
			"\tSTR R0, R6, #-1\n"
			"%s%i\n"
			"\tADD R6, R6, #-1\n\n", fbname, bnum, fbname, bnum + 1, fbname, bnum, fbname, bnum + 1);
		bnum += 3;
	} else if (tok->type == GE) {
		// printf("INSIDE LE WRITER");
		fprintf(f, "\t;; GE\n");
		fprintf(f, 
			"\tLDR R0, R6, 0\n"
			"\tLDR R1, R6, 1\n"
			"\tADD R6, R6, #2\n"
			"\tCMP R0, R1\n"
			"\tBRn %s%i\n" 
			"\tCONST R0, #1\n"
			"\tSTR R0, R6, #-1\n"
			"\tJMP %s%i\n"
			"%s%i\n"
			"\tCONST R0, #0\n"
			"\tSTR R0, R6, #-1\n"
			"%s%i\n"
			"\tADD R6, R6, #-1\n\n", fbname, bnum, fbname, bnum + 1, fbname, bnum, fbname, bnum + 1);
		bnum += 3;
	} else if (tok->type == GT) {
		// printf("INSIDE GT WRITER");
		fprintf(f, "\t;; GT\n");
		fprintf(f, 
			"\tLDR R0, R6, 0\n"
			"\tLDR R1, R6, 1\n"
			"\tADD R6, R6, #2\n"
			"\tCMP R0, R1\n"
			"\tBRnz %s%i\n" 
			"\tCONST R0, #1\n"
			"\tSTR R0, R6, #-1\n"
			"\tJMP %s%i\n"
			"%s%i\n"
			"\tCONST R0, #0\n"
			"\tSTR R0, R6, #-1\n"
			"%s%i\n"
			"\tADD R6, R6, #-1\n\n", fbname, bnum, fbname, bnum + 1, fbname, bnum, fbname, bnum + 1);
		bnum += 3;
	} else if (tok->type == EQ) {
		// printf("INSIDE GT WRITER");
		fprintf(f, "\t;; EQ\n");
		fprintf(f, 
			"\tLDR R0, R6, 0\n"
			"\tLDR R1, R6, 1\n"
			"\tADD R6, R6, #2\n"
			"\tCMP R0, R1\n"
			"\tBRnp %s%i\n" 
			"\tCONST R0, #1\n"
			"\tSTR R0, R6, #-1\n"
			"\tJMP %s%i\n"
			"%s%i\n"
			"\tCONST R0, #0\n"
			"\tSTR R0, R6, #-1\n"
			"%s%i\n"
			"\tADD R6, R6, #-1\n\n", fbname, bnum, fbname, bnum + 1, fbname, bnum, fbname, bnum + 1);
		bnum += 3;
	} else if (tok->type == IF) {
		if (top_if == NULL) {
			top_if = malloc(sizeof(*top_if));
			top_if->num = ifnum;
			ifnum ++;
			top_if->had_else = 0;
			top_if->next = NULL;
		} else {
			if_state *new_if = malloc(sizeof(*new_if));
			new_if->num = ifnum;
			ifnum ++;
			new_if->had_else = 0;
			new_if->next = top_if;
			top_if = new_if;
		}
		fprintf(f, "\t;; IF\n");
		fprintf(f, 
			"\tSTR R0, R6, #0\n"
			"\tADD R6, R6, #1\n"
			"\tCMPI R0, 1\n"
			"\tBRnp %s_else%i\n"
			, fname, top_if->num);
	} else if(tok->type == ELSE) {
		fprintf(f, "\t;; ELSE\n");
		fprintf(f, "\tJMP %s_endif%i\n", fname, top_if->num);
		fprintf(f, "%s_else%i\n", fname, top_if->num);
		top_if->had_else = 1;
	} else if(tok->type == ENDIF) {
		if (top_if->had_else == 0) {
			fprintf(f, ";; ELSE\n");
			fprintf(f, "%s_else%i\n", fname, top_if->num);
		} 
		fprintf(f, "\t;; ENDIF\n");
		fprintf(f, "%s_endif%i\n", fname, top_if->num);
		if_state *temp;
	    temp = top_if->next;
	    free(top_if);
	    top_if = temp; 
	} else if (tok->type == ARG) {
		fprintf(f, "\t;; ARG\n");
		fprintf(f, 
			"\tLDR R0, R5, %i\n"
			"\tSTR R0, R6, #-1\n"
			"\tADD R6, R6, #-1\n", tok->arg_no + 2);
	} else if (tok->type == RETURN) {
		fprintf(f, 
			"\tLDR R7, R6, #0\n"
			"\tSTR R7, R5, #2\n"
			"\tADD R6, R5, #0\n"
			"\tLDR R5, R6, #0\n"
			"\tLDR R7, R6, #1\n"
			"\tADD R6, R6, #2\n"
			"\tRET\n");
	}
	free(tok);
}
