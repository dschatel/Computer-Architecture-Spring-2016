#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "format.h"

//Flips bits and adds one to the bitstring.
char * TwosComplement (char * bitstring) {
	
	int i;
	char carry = '1';
	
	for (i = 0; i < strlen(bitstring); i++) {
		if (bitstring[i] == '0')
			bitstring[i] = '1';
		else if(bitstring[i] == '1')
			bitstring[i] = '0';
	}
	
	for (i = strlen(bitstring) - 1; i >= 0; i--) {
		
		if (bitstring[i] == '0' && carry == '0')
			bitstring[i] = '0';
		else if (bitstring[i] == '0' && carry == '1') {
			bitstring[i] = '1';
			carry = '0';
		}
		else if (bitstring[i] == '1' && carry == '1') {
			bitstring[i] = '0';
			carry = '1';
		}

	}
	
	return bitstring;
	
}

//Converts a 32-bit length ASCII Bitstring to an integer value
int BitStringToInt (char * bitstring) {
	
	int i;
	int sum = 0;
	
	for(i = 0; i < strlen(bitstring); i++) {
		
		sum = sum*2;
		sum = sum + bitstring[i] - '0';
		
	}
	
	return sum;
}

//Returns the number of digits in the integer for purposes of building the ASCII string
int IntOutPutLength (int val) {
	
	int count = 0;
	
	while (val != 0) {
		val = val/10;
		count++;
	}
	
	return count;
	
}

//Converts an integer to an ASCII string
void intToDecASCII (int x, char *s) {
	
	int pos;
	int isNegative;
	int i;
	
	if (x < 0) {isNegative = 1; x*= -1;}
	else		{isNegative = 0;}
	
	pos = 0;
	
	do {
		s[pos++] = x%10 + '0';
		x = x/10;
	} while (x > 0);
	
	if (isNegative) s[pos++] = '-';
	
	int limit = pos/2;
	char temp;
	
	for (i = 0; i< limit; i++) {
		temp = s[i];
		s[i] = s[pos-i-1];
		s[pos-i-1] = temp;
	}
	s[pos++] = '\0';
	
}


//Converts a 32-bit length ASCII Bitstring to a float value
float BitStringToFloat (char * bitstring) {
	int sign;
	float exponent = 0;
	float mantissa;
	int i;
	
	if(bitstring[0] == '0')
		sign = 0;
	else if (bitstring[0] == '1')
		sign = 1;
	
	sign = pow(-1, sign);
	
	for(i = 1; i <= 8; i++) {
		exponent = exponent * 2;
		exponent = exponent + bitstring[i] - '0';
	}
	
	
	exponent = exponent - 127;
	exponent = pow(2, exponent);
	
	int power = -1;
	
	for(i = 9; i < strlen(bitstring); i++) {
		
		mantissa = mantissa + (bitstring[i] - '0')* pow(2, power);
		power--;
		
	}
	
	mantissa = mantissa + 1;
	
	
	float finalVal = sign * mantissa * exponent;
	
	
	return finalVal;
}

//Evaluates a potential float number for exceptional cases: Zero, + or - Infinity, or + or - NaN
enum floatType evalFloat (char * bitstring) {
	
	int i = 0;;
	int isNegative = 0;
	int OddExp = 0;
	int NonZeroMantissa = 0;
	
	if(bitstring[i] == '1')
		isNegative = 1;
	
	for(i = 1; i < 9; i++) {
		if (bitstring[i] == '0') {
			OddExp = 0;
		}
		else
			OddExp = 1;
	}

		for (i = 9; i < strlen(bitstring); i++) {
		if (bitstring[i] == '1') {
			NonZeroMantissa = 1;
			break;
		}
	}	
	
	//printf("isNegative: %i OddExp: %i NonZeroMantissa: %i\n", isNegative, OddExp, NonZeroMantissa);
	
	if (isNegative == 0 && OddExp == 1 && NonZeroMantissa == 0)
		return Pinf;
	else if (isNegative == 0 && OddExp == 1 && NonZeroMantissa == 1)
		return PNan;
	else if (isNegative == 1 && OddExp == 0 && NonZeroMantissa == 0)
		return NZero;
	else if (isNegative == 1 && OddExp == 1 && NonZeroMantissa == 0)
		return Ninf;
	else if (isNegative == 1 && OddExp == 1 && NonZeroMantissa == 1)
		return NNan;
	else
		return Normal;
	
}

// Convert IEEE 32-bit floating point to printable ASCII string

// input: x is the 32-bit input.
// output:  digit is the output ASCII character
// output:  pow10 is the original pre-normalization power of 10.
// output: remainder is x after subtracting the higi-digit value

static void
get_float_digit( float x, char * digit, int * pow10, float * remainder )
{
	int			p10;

	if ( x == 0.0 )
	{
		*digit = '0';			// digit is always '0'
		*pow10 = 0;
		*remainder = 0.0;
	}
	else
	{
		*pow10 = 0;			// start with 10^0
		while ( x >= 10.0 )		// reduce
		{
			x /= 10.0;		// large values get smaller
			*pow10 += 1;
		}
		while ( x < 1.0 )		// increase
		{
			x *= 10.0;		// small values get larger
			*pow10 -= 1;
		}
		*digit = '0';
		do {				// 10.0 > x >= 1.0
			x -= 1.0;
			*digit += 1;		// digit is always non-'0'zero
		} while ( x >= 1.0 );
		p10 = 0;
		while ( p10 > *pow10 )		// leading digit is now removed from x
		{
			x /= 10;
			p10 -= 1;
		}
		while ( p10 < *pow10 )
		{
			x *= 10;
			p10 += 1;
		}
		*remainder = x;
	}
}

// Improve efficiency by adding more while loops to handle larger powers of 10, e.g. :
// while ( x >= 1e1024 ) { x /= 1e0124; pow10 += 1024; }
// while ( x >= 1e512 ) { x /= 1e512; pow10 += 512; }
// ...
// while ( x >= 10.0 ) { x /= 10.0 ; pow10 += 1; }
// And
// while ( x < 1.0 ) { x *= 10; pow10 -= 1; }
// ...
// while ( x < 1e-512 ) { x *= 1e512; pow10 -= 512; }
// while ( x < 1e-1024 ) { x *= 1e1024; pow10 -= 1024; }

static void
append( char * s, char c )
{
	char		buf[2];

	buf[0] = c;
	buf[1] = '\0';
	strcat( s, buf );
}

void
floatToASCII( float x, char * output )
{
	char		c;
	int		pow10, p10, plast;
	int		i;
	float		remainder;
	char		exponent[10];

	output[0] ='\0';
	if ( x < 0.0 )
	{
		append( output, '-' );
		x = -x;					// make x positive
	}
	get_float_digit( x, &c, &pow10, &remainder );
	append( output, c );
	append( output, '.' );
	x = remainder;
	plast = p10 = pow10;			// pow10 set by get_float_digit()
	for ( i = 1 ; i < 7 ; i++ )		// 7 significant digits in 32-bit float
	{
		get_float_digit( x, &c, &p10, &remainder );
		if ( (plast - p10) > 1 )
		{
			append( output, '0' );	// fill in zero to next nonzero digit
			plast -= 1;
		}
		else
		{
			append( output, c );
			x = remainder;
			plast = p10;
		}
	}
	if ( pow10 < 0 )		// negative exponent
	{
		exponent[0] = 'e';
		intToDecASCII( pow10, exponent+1 );
	}
	else if ( pow10 < 10 )		// positive single-digit exponent
	{
		exponent[0] = 'e';
		exponent[1] = '+';
		exponent[2] = '0';
		intToDecASCII( pow10, exponent+3 );
	}
	else				// positive multi-digit exponent
	{
		exponent[0] = 'e';
		exponent[1] = '+';
		intToDecASCII( pow10, exponent+2 );
	}
	strcat( output, exponent );
}

int validBitstring (char * bitstring) {
	int i;
	
	for(i = 0; i < strlen(bitstring); i++) {
		if (bitstring[i] != '0' && bitstring[i] != '1')
			return 1;
	}
	
	return 0;
}

int main(int argc, char **argv) {
	
	if (argc < 3) {
		fprintf(stderr, "ERROR: Too Few Arguments.\n");
		return 1;
	}
	else if (argc > 3) {
		fprintf(stderr, "ERROR: Too Many Arguments.\n");
		return 1;
	}

	char * string = argv[1];
	char * output = argv[2];
	
	if (strlen(string) < 32) {
		fprintf(stderr, "ERROR: Bit String is too short.\n");
		return 1;
	}
	else if (strlen(string) > 32) {
		fprintf(stderr, "ERROR: Bit String is too long.\n");
		return 1;
	}
	
	if (validBitstring(string) == 1) {
		fprintf(stderr, "ERROR: Bit String invalid. Please use only 1's and 0's.\n");
		return 1;
	}

	int isNegative = 0;
	
	if (strcmp(output, "int") == 0) {
		int finalVal;
		if(string[0] == '1') {
			isNegative = 1;
			string = TwosComplement(string);
		}
	
		finalVal = BitStringToInt(string);
			
		if(isNegative == 1)
			finalVal = finalVal * -1;
		
		char * finalString = (char *) malloc (sizeof(char) * IntOutPutLength(finalVal) + 1);
		
		intToDecASCII(finalVal, finalString);
	
		printf("%s\n", finalString);
	}
	else if(strcmp(output, "float") == 0) {
		
		enum floatType typeOfFloat = evalFloat(string);
		
		if (typeOfFloat == NZero)
			printf("-0.0e0\n");
		else if (strcmp(string, "00000000000000000000000000000000") == 0)
			printf("0.0e0\n");
		else if (typeOfFloat == Pinf)
			printf("+Inf\n");
		else if (typeOfFloat == Ninf)
			printf("-Inf\n");
		else if (typeOfFloat == PNan)
			printf("+NaN\n");
		else if (typeOfFloat == NNan)
			printf("-NaN\n");
		else {		
			float finalVal = BitStringToFloat(string);
			char * finalString = malloc(sizeof(char));
			
			floatToASCII(finalVal, finalString);
			printf("%s\n", finalString);
		}
	}
	else {
		fprintf(stderr, "ERROR: Incorrect input type. Please use 'int' or 'float'.\n");
		return 1;
	}
	
	return 0;
	
}