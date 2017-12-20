// contains your routines for decoding instructions


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "token.h"
#include "parser.h"

#define MAX_TOKEN_LENGTH 250
// #define MAX_LINE_LENGTH 300

void loadTokens(FILE *file) {

	int defFun = 0;
	int lit = 0;

	char tok[MAX_TOKEN_LENGTH];
	int endBool = 0;

	char eof;

	while (endBool == 0) {
		if (fscanf(file, "%c", &eof) == EOF) {
			break;
		}
		else {
			fseek(file, -1, SEEK_CUR);
		}

		if (fscanf(file, "%i", &lit) == 1 ) {
			token *tok;
			tok = malloc(sizeof(*tok));
			tok->type = LITERAL;
			tok->literal_value = lit;
			printf("%d\n", lit);
			writeToken(tok);
			// free(tok);

		} else {
			fseek(file, -1, SEEK_CUR);
			if (fscanf(file, "%s", tok) == 1) {
				// printf("Word: %s\n", tok);
				if (tok[0] == ';') {
					
					char ch = 'i';
					fscanf(file, "%c", &ch);
					while (ch != '\n') {
						fscanf(file, "%c", &ch);
					}
				}
				else if ((tok[0] == '0') & (tok[1] == 'x')) {
					token *t;
					t = malloc(sizeof(*t));
					t->type = LITERAL;
					int i;
					sscanf(tok, "%i", &i);
					t->literal_value = i;
					writeToken(t);
				}
				else {

					printf("%s\n", tok);
					token *tk; 
					tk = malloc(sizeof(*tk));
					char word[MAX_TOKEN_LENGTH];
					// strcpy(word, "-");
					// printf("Word: %s\n", word);
					if ((char)tok[0] == '+') {
						// printf("INSIDE IS TYPE PLUS\n");
						tk->type = PLUS;
						writeToken(tk);
						continue;
					}
					// strcpy(word, "-");
					if ((char)tok[0] == '-') {
						// printf("INSIDE IS TYPE MINUS\n");
						tk->type = MINUS;
						writeToken(tk);
						continue;
					}
					if ((char)tok[0] == '*') {
						// printf("INSIDE IS TYPE MINUS\n");
						tk->type = MUL;
						writeToken(tk);
						continue;
					}
					if ((char)tok[0] == '/') {
						// printf("INSIDE IS TYPE MINUS\n");
						tk->type = DIV;
						writeToken(tk);
						continue;
					}

					if ((char)tok[0] == '%') {
						// printf("INSIDE IS TYPE MINUS\n");
						tk->type = MOD;
						writeToken(tk);
						continue;
					}
					strcpy(word, "drop");

					if (strcmp(tok, word) == 0) {
						tk->type = DROP;
						writeToken(tk);
						continue;
					}
					strcpy(word, "swap");
					if (strcmp(tok, word) == 0) {
						tk->type = SWAP;
						writeToken(tk);
						continue;
					}

					strcpy(word, "dup");
					if (strcmp(tok, word) == 0) {
						tk->type = DUP;
						writeToken(tk);
						continue;
					}

					strcpy(word, "rot");
					if (strcmp(tok, word) == 0) {
						tk->type = ROT;
						writeToken(tk);
						continue;
					}
					
					strcpy(word, "lt");
					if (strcmp(tok, word) == 0) {
						tk->type = LT;
						writeToken(tk);
						continue;
					}

					strcpy(word, "le");
					if (strcmp(tok, word) == 0) {
						tk->type = LE;
						writeToken(tk);
						continue;
					}
					strcpy(word, "eq");
					if (strcmp(tok, word) == 0) {
						tk->type = EQ;
						writeToken(tk);
						continue;
					}
					strcpy(word, "ge");
					if (strcmp(tok, word) == 0) {
						tk->type = GE;
						writeToken(tk);
						continue;
					}
					strcpy(word, "gt");
					if (strcmp(tok, word) == 0) {
						tk->type = GT;
						writeToken(tk);
						continue;
					}
					strcpy(word, "if");
					if (strcmp(tok, word) == 0) {
						tk->type = IF;
						writeToken(tk);
						continue;
					}
					strcpy(word, "else");
					if (strcmp(tok, word) == 0) {
						tk->type = ELSE;
						writeToken(tk);
						continue;
					}
					strcpy(word, "endif");
					if (strcmp(tok, word) == 0) {
						tk->type = ENDIF;
						writeToken(tk);
						continue;
					}
					

					strcpy(word, "defun");
					if (strcmp(tok, word) == 0) {
						defFun = 1;
						free(tk);
						continue;
					}
					strcpy(word, "arg");
					if ((tok[0] == 'a') & (tok[1] == 'r') & (tok[2] == 'g')) {
						tk->type = ARG;
						int no;

						sscanf(tok, "arg%i", &no);
						tk->arg_no = no;
						writeToken(tk);
						continue;
					}

					strcpy(word, "return");
					if (strcmp(tok, word) == 0) {
						tk->type = RETURN;
						writeToken(tk);
						continue;
					}

					if (defFun) {
						tk->type = IDENT;
						// strcpy(tk->str, tok);
						sscanf(tok, "%s", tk->str);
						writeToken(tk);
						defFun = 0;
					} else {
						tk->type = IDENT;
						// strcpy(tk->str, tok);
						sscanf(tok, "%s", tk->str);
						jumpMethod(tk);
					}
					// strcpy(word, "*");
					// if (strcmp(tok, word)) {
					// 	tk->type = MUL;
					// 	writeToken(tk);
					// }
					// strcpy(word, "/");
					// if (strcmp(tok, word)) {
					// 	tk->type = DIV;
					// 	writeToken(tk);
					// }
					// strcpy(word, "%%");
					// if (strcmp(tok, word)) {
					// 	tk->type = MOD;
					// 	writeToken(tk);
					// }
					// printf("Didn't find anything");
				}
			} else {
				endBool = 1;
			}
		}
	}
}