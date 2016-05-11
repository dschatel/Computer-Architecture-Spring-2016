all:    formula

formula:         formula.c formula.h nCr.h nCr.s
		gcc -Wall -g -o formula formula.c nCr.s -m32

clean:
		rm -rf *.o formula