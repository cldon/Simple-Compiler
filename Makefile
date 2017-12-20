jc : main.c token.c token.h parser.c parser.h
	clang -lm -Wall -g -o jc main.c parser.c token.c
clean :
	rm jc *.o
