//Types for special cases
enum floatType {Normal, PZero, NZero, Pinf, Ninf, PNan, NNan};

//Converting from bitstring to an int or float value
char * TwosComplement (char * bitstring);
int BitStringToInt (char * bitstring);
float BitStringToFloat (char * bitstring);

//For converting from an int value to an ASCII equivalent
int IntOutPutLength (int val);
void intToDecASCII (int x, char *s);

//Dr. Russell's Functions for converting a float value to its ASCII equivalent
static void get_float_digit( float x, char * digit, int * pow10, float * remainder );
static void append( char * s, char c );
void floatToASCII( float x, char * output );

//Error checking functions
int validBitstring (char * bitstring);
enum floatType evalFloat (char * bitstring);