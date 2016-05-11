all:    y86emul y86dis

y86emul:         y86emul.c y86emul.h
		gcc -Wall -g -o y86emul y86emul.c -std=c99 -lm

y86dis:        y86dis.c y86dis.h
		gcc -Wall -g -o y86dis y86dis.c -std=c99 -lm

clean:
		rm -rf *.o y86emul y86dis