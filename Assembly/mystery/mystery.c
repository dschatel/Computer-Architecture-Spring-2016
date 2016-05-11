#include <stdio.h>
#include <stdlib.h>
#include "mystery.h"

int num[200];

int add (int x, int y) {
	
	return x + y;
}

int dothething (int n) {
	
	if(num[n] == -1) {
		
		if (n == 0)
			num[n] = 0;
		else if (n == 1)
			num[n] = 1;
		else
			num[n] = add(dothething(n-1), dothething(n-2));
		
	}
	
	return num[n];
	
}

int main(int argc, char **argv) {
	
	int n = atoi(argv[1]);
	int i = 0;
	
	while (i < 200) {
		num[i] = -1;
		i++;
	}
	
	printf ("%d\n", dothething(n));
	
	return 0;
	
}