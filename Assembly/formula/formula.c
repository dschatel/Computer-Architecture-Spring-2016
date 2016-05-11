#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/time.h>
#include "nCr.h"
#include "formula.h"

int legal_int(char *str) {
	
	while (*str) {
		if (!isdigit(*str++))
			return 1;
	}
	return 0;

}

int main(int argc, char **argv) {
	
	if (argc > 2) {
		fprintf(stderr, "ERROR: Too many arguments.\n");
		return 1;
	}
	else if (argc < 2) {
		fprintf(stderr, "ERROR: Too few arguments.\n");
		return 1;
	}
		
	if(strcmp(argv[1], "-h") == 0) {
		printf("Usage: formula <positive integer>\n");
		return 0;
	}
	else if (legal_int(argv[1]) == 0) {
	
		struct timeval start, end;
	
		gettimeofday(&start, NULL);
	
		int n = atoi(argv[1]);
		int counter = 0;
	
		if (nCr(n, counter) == 0) {
				fprintf(stderr, "Overflow Error!\n");
				return 1;
			}
	
		printf("(1+x)^%d = ", n);
	
		while(counter <= n) {
			if(counter == 0)
				printf("1");
			else {
				int ncr = nCr(n, counter);
				printf(" + %d*x^%d", ncr, counter);
			}
			counter++;
		}
	
		printf("\n");
	
		gettimeofday(&end, NULL);
	
		printf("Time Required = %ld microseconds\n", (end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec));
	
		return 0;
	}
	else {
		fprintf(stderr, "Unrecognized input. Please use '-h' for usage instructions.\n");
		return 1;
	}
		
	
}