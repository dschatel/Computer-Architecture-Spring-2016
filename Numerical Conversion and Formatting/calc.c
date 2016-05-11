#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "calc.h"

//Evaluates Input Type from string and returns Enum.
enum inputType formattype (char c) {
	
	if (c == 'b')
		return Binary;
	else if (c == 'o')
		return Octal;
	else if (c == 'x')
		return Hex;
	else if (c == 'd')
		return Decimal;
	else
		return Invalid;
	
}

//Flips bits in integer array and adds 1 to the bitstring. Called if the input string is preceded by a '-'.
void TwosComplement (int * num, int size) {

	int carry = 1;
	int i;
	
	for (i = 0; i < size; i++) {
		if (num[i] == 0)
			num[i] = 1;
		else
			num[i] = 0;
	}
	
	for (i = size - 1; i >= 0; i--) {
		num[i] += carry;
		if(num[i] > 1) {
			num[i]-=2;
			carry = 1;
		}
		else
			carry = 0;
		
			
	}
	
}

//Creates an ArbitraryInt object and sets all of the struct variables depending on the content of the input string.
ArbitraryInt * CreateInt (char * input) {
	
	ArbitraryInt *newInt = malloc(sizeof(ArbitraryInt));
	char * newstring;
	
	if(input[0] == '-') {
		newInt->isnegative = 1;
		newInt->type = formattype(input[1]);
		newstring = input+2;
	}
	else {
		newInt->isnegative = 0;
		newInt->type = formattype(input[0]);
		newstring = input + 1;
	}
	
	if(newInt->isnegative == 1) {
		newInt->ASCIIString =  malloc(sizeof (char) * strlen(input) - 2);
		strcpy(newInt->ASCIIString, newstring);
	}
	else {
		newInt->ASCIIString =  malloc(sizeof (char) * strlen(input) - 1);
		strcpy(newInt->ASCIIString, newstring);
	}

	return newInt;
}

//Converts the ASCII Binary Input to an Integer array of bits
void ASCIIBinToInt (ArbitraryInt * num) {
	
	int i;
	num->size = strlen(num->ASCIIString) + 2;
	num->bigarray = (int*) malloc(sizeof(int) * num->size);
	
	for(i=0; i < strlen(num->ASCIIString); i++) {
		num->bigarray[i+2] = num->ASCIIString[i] - '0';
	}
	
	num->bigarray[1] = 0;
	num->bigarray[0] = 0;
	
	if (num->isnegative == 1)
		TwosComplement(num->bigarray, num->size);
}

//Evaluates individual octal characters and returns a string of equivalent binary
char * ASCIIOctToASCIIBin (char c) {
	switch(c) {
		case '0': return "000"; break;
		case '1': return "001"; break;
		case '2': return "010"; break;
		case '3': return "011"; break;
		case '4': return "100"; break;
		case '5': return "101"; break;
		case '6': return "110"; break;
		case '7': return "111"; break;
		default: return "X";
	}
}

//Evaluates individual hex characters and returns a string of equivalent binary
char * ASCIIHexToASCIIBin (char c) {
	switch(c) {
		case '0': return "0000"; break;
		case '1': return "0001"; break;
		case '2': return "0010"; break;
		case '3': return "0011"; break;
		case '4': return "0100"; break;
		case '5': return "0101"; break;
		case '6': return "0110"; break;
		case '7': return "0111"; break;
		case '8': return "1000"; break;
		case '9': return "1001"; break;
		case 'A': 
		case 'a': return "1010"; break;
		case 'B': 
		case 'b': return "1011"; break;
		case 'C': 
		case 'c': return "1100"; break;
		case 'D': 
		case 'd': return "1101"; break;
		case 'E': 
		case 'e': return "1110"; break;
		case 'F': 
		case 'f': return "1111"; break;
		default: return "X";
	}
}

//Converts an ASCII Octal input to an Integer array of bits
void ASCIIOctToInt (ArbitraryInt * num) {
	
	int j = 2;	
	int i = 0;
	
	num->size = (strlen(num->ASCIIString) * 3) + 2;
	num->bigarray = (int*) malloc(sizeof(int) * num->size);
	
	for (; i < strlen(num->ASCIIString); i++) {
		char * digit = ASCIIOctToASCIIBin(num->ASCIIString[i]);
		num->bigarray[j] = digit[0] - '0';
		num->bigarray[j+1] = digit[1] - '0';
		num->bigarray[j+2] = digit[2] - '0';
		j+=3;
	}
	
	num->bigarray[1] = 0;
	num->bigarray[0] = 0;
	
	if (num->isnegative == 1)
		TwosComplement(num->bigarray, num->size);
	
}

//Converts an ASCII Hex input to an Integer array of bits
void ASCIIHexToInt (ArbitraryInt * num) {
	
	int j = 2;
	int i;
	
	num->size = (strlen(num->ASCIIString) * 4) + 2;
	num->bigarray = (int*) malloc(sizeof(int) * num->size);	
	
	for (i = 0; i < strlen(num->ASCIIString); i++) {
		char * digit = ASCIIHexToASCIIBin(num->ASCIIString[i]);
		num->bigarray[j] = digit[0] - '0';
		num->bigarray[j+1] = digit[1] - '0';
		num->bigarray[j+2] = digit[2] - '0';
		num->bigarray[j+3] = digit[3] - '0';
		j+=4;
	}
	
	num->bigarray[1] = 0;
	num->bigarray[0] = 0;
	
	if (num->isnegative == 1)
		TwosComplement(num->bigarray, num->size);
	
}

//Converts an ASCII Decimal number to its corresponding value in int
int asciitoint (char *s) {
	int val = 0;
	int pos = 0;
	
	while(s[pos] != '\0') {
		val = 10*val + s[pos] - '0';
		pos+=1;
	}

	return val;
}

//Returns the length of a decimal for purposes of creating the appropriate-sized integer array
int DecLength (int val) {
	int length = 0;
	
	while (val != 0) {
		val = val/2;
		length++;
	}
	return length;
}

//Converts an ASCII Decimal input to an Integer array of bits
void IntToBin (ArbitraryInt * num) {
	int val = asciitoint(num->ASCIIString);
	int j;
	
	num->size = DecLength(val) + 2;
	num->bigarray = (int*) malloc(sizeof(int) * num->size);
	
	j = num->size -1;

	while (val != 0) {
		num->bigarray[j] = val % 2;
		val = val/2;
		j--;
	}
	
	num->bigarray[1] = 0;
	num->bigarray[0] = 0;
	
	if (num->isnegative == 1)
		TwosComplement(num->bigarray, num->size);
	
}

//Adds leading bits to the front of the smaller of the two bitstrings in order to assist in arithmetic
void evenBinary (ArbitraryInt * firstNum, ArbitraryInt * secondNum) {
	
	int * newArray;
	int leading;
	int i;
	int j;
	ArbitraryInt * smaller;
	ArbitraryInt * greater;
	
	if (firstNum -> size > secondNum->size) {
		greater = firstNum;
		smaller = secondNum;
	}
	else {
		greater = secondNum;
		smaller = firstNum;
	}
	
	newArray = malloc(sizeof(int) * greater->size);
	
	leading = smaller->bigarray[0];
	i = 0;
	
	while (i < (greater->size - smaller ->size)) {
		newArray[i] = leading;
		i++;
	}
	
	j = 0;
	
	while (i < greater->size) {
		newArray[i] = smaller->bigarray[j];
		i++;
		j++;
	}
	
	smaller->size = greater->size;
	smaller->bigarray = newArray;

}

//Adds two bitstrings together
void add (ArbitraryInt * firstNum, ArbitraryInt * secondNum, int * result) {

	int carry = 0;
	int size = firstNum->size;
	int i;
		
	for (i = size-1; i >=0; i--) {
		
		result[i] = firstNum->bigarray[i] + secondNum->bigarray[i] + carry;
		if (result[i] > 1) {
			result[i]-=2;
			carry = 1;
		}
		else
			carry = 0;
	}
}

//Subtracts two bitstrings
void subtract (ArbitraryInt * firstNum, ArbitraryInt * secondNum, int * result) {
	
	int borrow = 0;
	int size = firstNum->size;
	int i;
	
	for(i = size-1; i >= 0; i--) {
		result[i] = firstNum->bigarray[i] - borrow - secondNum->bigarray[i];
		if (result[i] < 0) {
			result[i]+=2;
			borrow = 1;
		}
		else
			borrow = 0;
	}
}

//Evaluates a binary string and returns the corresponding Octal character
char singleOctDigit (char * digit) {
		
		if (strcmp(digit, "000") == 0)
			return '0'; 
		else if (strcmp(digit, "001") == 0)
			return '1';
		else if (strcmp(digit, "010") == 0)
			return '2';
		else if (strcmp(digit, "011") == 0)
			return '3';
		else if (strcmp(digit, "100") == 0)
			return '4';
		else if (strcmp(digit, "101") == 0)
			return '5';
		else if (strcmp(digit, "110") == 0)
			return '6';
		else if (strcmp(digit, "111") == 0)
			return '7';
		else
			return 'X';
	}

//Converts from an integer array of bits to Octal ASCII
char * IntToOctASCII (int * binary, int size) {
	
	char * result;
	char * singledigit;
	int i;
	int j;
	
	if (binary[0] == 1)
		TwosComplement(binary, size);

	result = (char *) malloc (sizeof(char) * (size / 3) + 1);
	
	singledigit = (char *) malloc (sizeof(char) * 3);
	
	i = 0;
	j = 0;	
	if ((size % 3) == 1) {
		singledigit[0] = '0';
		singledigit[1] = '0';
		singledigit[2] = binary[i] + '0';
		i++;
		result[j] = singleOctDigit(singledigit);
		j++;
	}
	else if ((size % 3) == 2) {
		singledigit[0] = '0';
		singledigit[1] = binary[i] + '0';
		singledigit[2] = binary[i+1] + '0';
		i+=2;
		result[j] = singleOctDigit(singledigit);
		j++;
	}


	
	while(i < size) {
		singledigit[0] = binary[i] +'0';
		singledigit[1] = binary[i+1] + '0';
		singledigit[2] = binary[i+2] + '0';
		result[j] = singleOctDigit(singledigit);
		j++;
		i+=3;
		
	}
	
	result[j] = '\0';
	
	return result;
}

//Evaluates a string of bits and returns the corresponding Hex character
char singleHexDigit (char * digit) {
		
		if (strcmp(digit, "0000") == 0)
			return '0'; 
		else if (strcmp(digit, "0001") == 0)
			return '1';
		else if (strcmp(digit, "0010") == 0)
			return '2';
		else if (strcmp(digit, "0011") == 0)
			return '3';
		else if (strcmp(digit, "0100") == 0)
			return '4';
		else if (strcmp(digit, "0101") == 0)
			return '5';
		else if (strcmp(digit, "0110") == 0)
			return '6';
		else if (strcmp(digit, "0111") == 0)
			return '7';
		else if (strcmp(digit, "1000") == 0)
			return '8';
		else if (strcmp(digit, "1001") == 0)
			return '9';
		else if (strcmp(digit, "1010") == 0)
			return 'A';
		else if (strcmp(digit, "1011") == 0)
			return 'B';
		else if (strcmp(digit, "1100") == 0)
			return 'C';
		else if (strcmp(digit, "1101") == 0)
			return 'D';
		else if (strcmp(digit, "1110") == 0)
			return 'E';
		else if (strcmp(digit, "1111") == 0)
			return 'F';
		else
			return 'X';
	}

//Converts from an integer array of bits to Hex ASCII
char * IntToHexASCII (int * binary, int size) {
	
	char * result;
	char * singledigit;
	int i;
	int j;
	
	if (binary[0] == 1)
		TwosComplement(binary, size);
		
	result = (char *) malloc (sizeof(char) * (size / 4) + 1);
	
	singledigit = (char *) malloc (sizeof(char) * 4);
	
	i = 0;
	j = 0;	
	if ((size % 4) == 1) {
		singledigit[0] = binary[i] + '0';
		singledigit[1] = binary[i] + '0';
		singledigit[2] = binary[i] + '0';
		singledigit[3] = binary[i] + '0';
		i++;
		result[j] = singleHexDigit(singledigit);
		j++;
	}
	else if ((size % 4) == 2) {
		singledigit[0] = binary[i] + '0';
		singledigit[1] = binary[i] + '0';
		singledigit[2] = binary[i] + '0';
		singledigit[3] = binary[i+1] + '0';
		i+=2;
		result[j] = singleHexDigit(singledigit);
		j++;
	}
	else if ((size % 4) == 3) {
		singledigit[0] = binary[i] + '0';
		singledigit[1] = binary[i] + '0';
		singledigit[2] = binary[i+1] + '0';
		singledigit[3] = binary[i+2] + '0';
		i+=3;
		result[j] = singleHexDigit(singledigit);
		j++;
	}

	while(i < size) {
		singledigit[0] = binary[i] +'0';
		singledigit[1] = binary[i+1] + '0';
		singledigit[2] = binary[i+2] + '0';
		singledigit[3] = binary[i+3] + '0';
		result[j] = singleHexDigit(singledigit);
		j++;
		i+=4;
		
	}
	
	result[j] = '\0';
	
	return result;
	
	
}

//Converts from an Integer array of bits to binary ASCII
char * IntToBinASCII (int * binary, int size) {
	
	char * result = (char *) malloc ((sizeof(char) * size) + 1);
	int i;
	
	if (binary[0] == 1)
		TwosComplement(binary, size);
	
	for (i = 0; i < size; i++) {
		result[i] = binary[i] + '0';
	}
	
	result[i] = '\0';
	return result;
}

//Returns the length of the anticipated Decimal string, for purposes of malloc
int DecOutPutLength (int val) {
	
	int count = 0;
	
	while (val != 0) {
		val = val/10;
		count++;
	}
	
	return count;
	
}

//Converts from an Integer array of bits to a Decimal ASCII
char * IntToDecASCII (int * binary, int size) {
	
	int sum = 0;
	int i;
	int j;
	int decLength;
	char * result;
	int remainder;
	
	if (binary[0] == 1)
		TwosComplement(binary, size);
		
	for(i = 0; i < size; i++) {
		sum = sum * 2;
		sum = sum + binary[i];
	}
	
	decLength = 1;
	
	if (sum != 0)
		decLength = DecOutPutLength(sum);
	
	result = (char *) malloc (sizeof(char) * decLength + 1);
	remainder = 0;
	
	result[decLength] = '\0';
	
	for(j = decLength-1; j >= 0; j--) {
		remainder = sum % 10;
		result[j] = remainder + '0';
		sum = sum / 10;
	}
	
	return result;
	
}

//Formats the input string based on input type
void formatNum(ArbitraryInt * num) {
	if(num->type == Binary)
		ASCIIBinToInt(num);
	else if(num->type == Octal)
		ASCIIOctToInt(num);
	else if(num->type == Hex)
		ASCIIHexToInt(num);
	else if(num->type == Decimal)
		IntToBin(num);
}

//Verifies the input string to ensure it is a valid Binary/Hex/Decimal/Octal
int verifyInput (ArbitraryInt * num) {
	int i;
	
	
	if (num->type == Binary) {
		for(i = 0; i < strlen(num->ASCIIString); i++) {
			if (num->ASCIIString[i] != '0' && num->ASCIIString[i] != '1')
				return 1;
		}
		return 0;
	}
	
	if(num->type == Octal) {
		for (i = 0; i < strlen(num->ASCIIString); i++) {
			if (!isdigit(num->ASCIIString[i]) || num->ASCIIString[i] == '8' || num->ASCIIString[i] == '9')
				return 1;
		}
		return 0;
	}
	
	if (num->type == Decimal) {
		for (i = 0; i < strlen(num->ASCIIString); i++) {
			if (!isdigit(num->ASCIIString[i]))
				return 1;
		}
		return 0;
	}
	
	if (num->type == Hex) {
		for (i = 0; i < strlen(num->ASCIIString); i++) {
			if (!isxdigit(num->ASCIIString[i]))
				return 1;
		}
		return 0;
	}
	
	return 0;
		
}

int main(int argc, char **argv) {
	
	char * operation;
	char * output;
	int * resultBinary;
	char * finaloutput;
	ArbitraryInt *firstNum;
	ArbitraryInt *secondNum;
	
	if (argc < 5) {
		fprintf(stderr, "ERROR: Too Few Arguments.\n");
		return 1;
	}
	
	operation = argv[1];
	
	if (operation[0] != '+' && operation [0] != '-') {
		fprintf(stderr, "ERROR: Unrecognized Arithmetic Operation. Please use only '+' or '-'.\n");
		return 1;
	}
	
	firstNum = CreateInt(argv[2]);
	secondNum = CreateInt(argv[3]);
	
	if(firstNum->type == Invalid) {
		fprintf(stderr, "ERROR: Unrecognized format type for First Number. Please use 'b' for Binary, 'd' for Decimal, 'x' for Hexadecimal, or 'o' for Octal.\n");
		return 1;
	}
	
	if(secondNum->type == Invalid) {
		fprintf(stderr, "ERROR: Unrecognized format type for Second Number. Please use 'b' for Binary, 'd' for Decimal, 'x' for Hexadecimal, or 'o' for Octal.\n");
		return 1;
	}
	
	if(verifyInput(firstNum) == 1) {
		fprintf(stderr, "ERROR: Invalid Numerical String in First Number. Please enter a valid number for that format type.\n");
		return 1;
	}
	
	if(verifyInput(secondNum) == 1) {
		fprintf(stderr, "ERROR: Invalid Numerical String in Second Number. Please enter a valid number for that format type.\n");
		return 1;
	}
	
	output = argv[4];
	
	if (output[0] != 'd' && output[0] != 'x' && output[0] != 'b' && output[0] != 'o') {
		fprintf(stderr, "ERROR: Invalid Output type. Please use 'b' for Binary, 'd' for Decimal, 'x' for Hexadecimal, or 'o' for Octal.\n");
		return 1;
	}
		
	formatNum(firstNum);
	formatNum(secondNum);
		
	evenBinary(firstNum, secondNum);
	
	resultBinary  = (int*) malloc((sizeof(int) * firstNum->size));
	
	if(operation[0] == '+')
		add(firstNum, secondNum, resultBinary);
	else
		subtract(firstNum, secondNum, resultBinary);
	
	if (resultBinary[0] == 1)
		printf("%c", '-');
	
	if(output[0] == 'o') {
		finaloutput = IntToOctASCII(resultBinary, firstNum->size);
	}
	else if(output[0] == 'b') {
		finaloutput = IntToBinASCII(resultBinary, firstNum->size);
	}
	else if(output[0] == 'x') {
		finaloutput = IntToHexASCII(resultBinary, firstNum->size);
	}
	else if(output[0] == 'd') {
		finaloutput = IntToDecASCII(resultBinary, firstNum->size);
	}
	
	printf(output);
	printf("%s\n", finaloutput);
	
	
	return 0;
	
}