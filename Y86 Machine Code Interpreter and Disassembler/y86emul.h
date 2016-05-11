#define AOK "AOK: Program Ran Successfully.\n"
#define HLT "HLT: Program encountered a Halt Instruction.\n"
#define ADR "ADR: Program encountered an invalid address.\n"
#define INS "INS: Program encountered an invalid instruction.\n"

#define EAX 0
#define ECX 1
#define EDX 2
#define EBX 3
#define ESP 4
#define EBP 5
#define ESI 6
#define EDI 7

struct Y86_ {
	
	int y86reg[8];
	
	int counter;
	int mem_size;
	
	int overflow_flag;
	int zero_flag;
	int negative_flag;
	
	unsigned char * memory;
	
};

typedef struct Y86_ Y86;

int single_digit_hex_to_dec (char c);
int hex_to_dec (char * hex);

int run_program(Y86 * program);
int setupY86 (char * input);
char * openFile (char * filename);