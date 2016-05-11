all:    calc format

calc:         calc.c calc.h
		gcc -Wall -g -o calc calc.c

format:        format.c format.h
		gcc -Wall -g -o format format.c -lm

clean:
		rm -rf *.o calc format