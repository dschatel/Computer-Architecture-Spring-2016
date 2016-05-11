//specifies the input type
enum inputType {Decimal, Octal, Hex, Binary, Invalid};

//Holds various data values for the input string
struct ArbitraryInt_ {
	int isnegative;
	int size;
	enum inputType type;
	char *ASCIIString;
	int * bigarray;
};

typedef struct ArbitraryInt_ ArbitraryInt;

//Create an ArbitraryInt object
ArbitraryInt * CreateInt (char * input);

//Misc functions to help get malloc size right
int DecLength (int val);
int DecOutPutLength (int val);

//Helper functions to prepare numbers for arithmetic
void formatNum(ArbitraryInt * num);
int verifyInput (ArbitraryInt * num);
void evenBinary (ArbitraryInt * firstNum, ArbitraryInt * secondNum);
enum inputType formattype (char c);
void TwosComplement (int * num, int size);


//Converting from ASCII Input to Integer Array
void ASCIIBinToInt (ArbitraryInt * num);
void ASCIIOctToInt (ArbitraryInt * num);
void ASCIIHexToInt (ArbitraryInt * num);
void IntToBin (ArbitraryInt * num);
char * ASCIIOctToASCIIBin (char c);
char * ASCIIHexToASCIIBin (char c);
int asciitoint (char *s);


//Arithmetic Operations
void add (ArbitraryInt * firstNum, ArbitraryInt * secondNum, int * result);
void subtract (ArbitraryInt * firstNum, ArbitraryInt * secondNum, int * result);

//Converting from Integer Array back to ASCII
char * IntToOctASCII (int * binary, int size);
char * IntToHexASCII (int * binary, int size);
char * IntToBinASCII (int * binary, int size);
char * IntToDecASCII (int * binary, int size);
char singleOctDigit (char * digit);
char singleHexDigit (char * digit);
