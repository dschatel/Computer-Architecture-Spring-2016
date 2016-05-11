#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "y86dis.h"

char * findReg(int regnum) {
	
	switch(regnum) {
		
		case 0: return "%EAX"; break;
		case 1: return "%ECX"; break;
		case 2: return "%EDX"; break;
		case 3: return "%EBX"; break;
		case 4: return "%ESP"; break;
		case 5: return "%EBP"; break;
		case 6: return "%ESI"; break;
		case 7: return "%EDI"; break;
		default: return "ERROR"; break;
		
	}
	
}

int single_digit_hex_to_dec (char c) {
	
	switch(c) {
		case '0': return 0; break;
		case '1': return 1; break;
		case '2': return 2; break;
		case '3': return 3; break;
		case '4': return 4; break;
		case '5': return 5; break;
		case '6': return 6; break;
		case '7': return 7; break;
		case '8': return 8; break;
		case '9': return 9; break;
		case 'A':
		case 'a': return 10; break;
		case 'B':
		case 'b': return 11; break;
		case 'C':
		case 'c': return 12; break;
		case 'D':
		case 'd': return 13; break;
		case 'E':
		case 'e': return 14; break;
		case 'F':
		case 'f': return 15; break;
		default: return 0; break;
		
	}
	
}

int hex_to_dec (char * hex) {
	
	int exponent = strlen(hex) - 1;
	int result = 0;
	
	for(int i = 0; i < strlen(hex); i++) {
		
		int hexval = single_digit_hex_to_dec(hex[i]);
		
		result += (hexval * (pow(16, exponent)));
		exponent--;
		
	}
	
	return result;
}

char * getInstructions (char * filename) {
	
		char * result = 0;
		int file_length;
	
		FILE * input_file = fopen (filename, "r");
		
		if(input_file) {
			fseek(input_file, 0, SEEK_END);
			file_length = ftell(input_file);
			fseek(input_file, 0, SEEK_SET);
			result = malloc(file_length + 1);
			
			if(result) {
				fread(result, 1, file_length, input_file);
			}
			
			result[file_length] = '\0';
			fclose(input_file);
		}
		else
			printf("File does not exist\n");
		
		if(result)	
			return result;
		else {
			printf("Something went wrong.\n");
			return "";
		}
		
		
}

int disassemble (char * program) {
	
	char * 	token = strtok(program, "\t\n");
	
	int memaddr;
	char * instructions;
	
	while (token != NULL) {
		
		//printf("%s\n", token);
		
		if(strcmp(token, ".text") == 0) {
			token = strtok(NULL, "\t\n");
			memaddr = hex_to_dec(token);
			//printf("%d is 0x%x\n", memaddr, memaddr);
			token = strtok(NULL, "\t\n");
			instructions = token;
			token = NULL;
		}
		else
			token = strtok(NULL, "\t\n");
	}
	
	//printf("Mem Address: %s\n", memaddr);
	//printf("Instruction String: %s\n", instructions);
	
	int size = strlen(instructions) / 2;
	
	//printf("Length of instruction string in bytes: %d\n", size);
	
	unsigned char * instruct = malloc(sizeof(unsigned char) * (size + 1));
	int i = 0;
	
	for (int j = 0; j < strlen(instructions); j+=2) {
				
				char * byte = malloc(sizeof(char) * 3);
				strncpy(byte, instructions + j, 2);
				byte[2] = '\0';
				
				//printf("Byte: %s\n", byte);
				
				unsigned char c = (unsigned char) hex_to_dec(byte);
				
				instruct[i] = c;
				//printf("This is in memory location %d: %x\n", i, instruct[i]);
				i++;
				
				
			}
	
		int reg_one;
		int reg_two;
		int value;
		char * register1;
		char * register2;
	
		for(int i = 0; i < size; i++, memaddr++) {
			
			printf("0x%x: ", memaddr);
			
			switch(instruct[i]) {
				
				case 0x00:
							printf("NOP\n");
							break;
				case 0x10:
							printf("HALT\n");
							break;
				case 0x20:
							reg_one = (instruct[i+1] & 0xf0) >> 4;
							reg_two = (instruct[i+1]& 0x0f);
							
							register1 = findReg(reg_one);
							register2 = findReg(reg_two);
							
							i++;
							memaddr++;
							
							printf("RRMOVL %s, %s\n", register1, register2);
							break;
				case 0x30:
				
							reg_two = (instruct[i+1]& 0x0f);
							
							register2 = findReg(reg_two);
							
							value = (instruct[i+5] << 24) +
								(instruct[i+4] << 16) +
								(instruct[i+3] << 8) +
								(instruct[i+2]);
							
							printf("IRMOVL $%d, %s\n", value, register2);
							i+=5;
							memaddr+=5;
							break;
				
				case 0x40: 
				
							reg_one = (instruct[i+1] & 0xf0) >> 4;
							reg_two = (instruct[i+1]& 0x0f);
							
							register1 = findReg(reg_one);
							register2 = findReg(reg_two);
							
							value = (instruct[i+5] << 24) +
								(instruct[i+4] << 16) +
								(instruct[i+3] << 8) +
								(instruct[i+2]);
							
							printf("RMMOVL %s, %d(%s)\n", register1, value, register2);
							i+=5;
							memaddr+=5;
							break;
				
				case 0x50:
							
							reg_one = (instruct[i+1] & 0xf0) >> 4;
							reg_two = (instruct[i+1]& 0x0f);
							
							register1 = findReg(reg_one);
							register2 = findReg(reg_two);
							
							value = (instruct[i+5] << 24) +
								(instruct[i+4] << 16) +
								(instruct[i+3] << 8) +
								(instruct[i+2]);
							
							printf("MRMOVL %d(%s), %s\n", value, register2, register1);
							i+=5;
							memaddr+=5;
							break;
				
				case 0x60: 
				
							reg_one = (instruct[i+1] & 0xf0) >> 4;
							reg_two = (instruct[i+1]& 0x0f);
							
							register1 = findReg(reg_one);
							register2 = findReg(reg_two);
							
							printf("ADDL %s, %s\n", register1, register2);
							i++;
							memaddr++;
							break;
							
				case 0x61: 
				
							reg_one = (instruct[i+1] & 0xf0) >> 4;
							reg_two = (instruct[i+1]& 0x0f);
							
							register1 = findReg(reg_one);
							register2 = findReg(reg_two);
							
							printf("SUBL %s, %s\n", register1, register2);
							i++;
							memaddr++;
							break;
							
				case 0x62: 
				
							reg_one = (instruct[i+1] & 0xf0) >> 4;
							reg_two = (instruct[i+1]& 0x0f);
							
							register1 = findReg(reg_one);
							register2 = findReg(reg_two);
							
							printf("ANDL %s, %s\n", register1, register2);
							i++;
							memaddr++;
							break;

				case 0x63: 
				
							reg_one = (instruct[i+1] & 0xf0) >> 4;
							reg_two = (instruct[i+1]& 0x0f);
							
							register1 = findReg(reg_one);
							register2 = findReg(reg_two);
							
							printf("XORL %s, %s\n", register1, register2);
							i++;
							memaddr++;
							break;
							
				case 0x64: 
				
							reg_one = (instruct[i+1] & 0xf0) >> 4;
							reg_two = (instruct[i+1]& 0x0f);
							
							register1 = findReg(reg_one);
							register2 = findReg(reg_two);
							
							printf("MULL %s, %s\n", register1, register2);
							i++;
							memaddr++;
							break;
							
				case 0x65: 
				
							reg_one = (instruct[i+1] & 0xf0) >> 4;
							reg_two = (instruct[i+1]& 0x0f);
							
							register1 = findReg(reg_one);
							register2 = findReg(reg_two);
							
							printf("CMPL %s, %s\n", register1, register2);
							i++;
							memaddr++;
							break;
							
				case 0x70:
				
							value = 	(instruct[i+4] << 24) +
										(instruct[i+3] << 16) +
										(instruct[i+2] << 8) +
										(instruct[i+1]);
										
							printf("JMP 0x%x\n", value);
							i+=4;
							memaddr+=4;
							break;
							
				case 0x71:
				
							value = 	(instruct[i+4] << 24) +
										(instruct[i+3] << 16) +
										(instruct[i+2] << 8) +
										(instruct[i+1]);
										
							printf("JLE 0x%x\n", value);
							i+=4;
							memaddr+=4;
							break;
							
				case 0x72:
				
							value = 	(instruct[i+4] << 24) +
										(instruct[i+3] << 16) +
										(instruct[i+2] << 8) +
										(instruct[i+1]);
										
							printf("JL 0x%x\n", value);
							i+=4;
							memaddr+=4;
							break;
							
				case 0x73:
				
							value = 	(instruct[i+4] << 24) +
										(instruct[i+3] << 16) +
										(instruct[i+2] << 8) +
										(instruct[i+1]);
										
							printf("JE 0x%x\n", value);
							i+=4;
							memaddr+=4;
							break;
							
				case 0x74:
				
							value = 	(instruct[i+4] << 24) +
										(instruct[i+3] << 16) +
										(instruct[i+2] << 8) +
										(instruct[i+1]);
										
							printf("JNE 0x%x\n", value);
							i+=4;
							memaddr+=4;
							break;
							
				case 0x75:
				
							value = 	(instruct[i+4] << 24) +
										(instruct[i+3] << 16) +
										(instruct[i+2] << 8) +
										(instruct[i+1]);
										
							printf("JGE 0x%x\n", value);
							i+=4;
							memaddr+=4;
							break;
							
				case 0x76:
				
							value = 	(instruct[i+4] << 24) +
										(instruct[i+3] << 16) +
										(instruct[i+2] << 8) +
										(instruct[i+1]);
										
							printf("JG 0x%x\n", value);
							i+=4;
							memaddr+=4;
							break;
							
				case 0x80:
				
							value = 	(instruct[i+4] << 24) +
										(instruct[i+3] << 16) +
										(instruct[i+2] << 8) +
										(instruct[i+1]);
										
							printf("CALL 0x%x\n", value);
							i+=4;
							memaddr+=4;
							break;
							
				case 0x90:
				
						printf ("RET\n");
						break;
				
				case 0xa0:
				
							reg_one = (instruct[i+1] & 0xf0) >> 4;
							
							register1 = findReg(reg_one);
							
							printf("PUSHL %s\n", register1);
							i++;
							memaddr++;
							break;
							
				case 0xb0:
				
							reg_one = (instruct[i+1] & 0xf0) >> 4;
							
							register1 = findReg(reg_one);
							
							printf("POPL %s\n", register1);
							i++;
							memaddr++;
							break;
							
				case 0xc0:
				
							reg_one = (instruct[i+1] & 0xf0) >> 4;
							
							register1 = findReg(reg_one);
							
							value = 	(instruct[i+5] << 24) +
										(instruct[i+4] << 16) +
										(instruct[i+3] << 8) +
										(instruct[i+2]); 
							
							printf("READB %d(%s)\n", value, register1);
							i+=5;
							memaddr+=5;
							break;
							
				case 0xc1:
				
							reg_one = (instruct[i+1] & 0xf0) >> 4;
							
							register1 = findReg(reg_one);
							
							value = 	(instruct[i+5] << 24) +
										(instruct[i+4] << 16) +
										(instruct[i+3] << 8) +
										(instruct[i+2]); 
							
							printf("READL %d(%s)\n", value, register1);
							i+=5;
							memaddr+=5;
							break;
							
				case 0xd0:
				
							reg_one = (instruct[i+1] & 0xf0) >> 4;
							
							register1 = findReg(reg_one);
							
							value = 	(instruct[i+5] << 24) +
										(instruct[i+4] << 16) +
										(instruct[i+3] << 8) +
										(instruct[i+2]); 
							
							printf("WRITEB %d(%s)\n", value, register1);
							i+=5;
							memaddr+=5;
							break;
							
				case 0xd1:
				
							reg_one = (instruct[i+1] & 0xf0) >> 4;
							
							register1 = findReg(reg_one);
							
							value = 	(instruct[i+5] << 24) +
										(instruct[i+4] << 16) +
										(instruct[i+3] << 8) +
										(instruct[i+2]); 
							
							printf("WRITEL %d(%s)\n", value, register1);
							i+=5;
							memaddr+=5;
							break;
							
				case 0xe0:
				
							reg_one = (instruct[i+1] & 0xf0) >> 4;
							reg_two = (instruct[i+1]& 0x0f);
							
							register1 = findReg(reg_one);
							register2 = findReg(reg_two);
							
							value = 	(instruct[i+5] << 24) +
										(instruct[i+4] << 16) +
										(instruct[i+3] << 8) +
										(instruct[i+2]); 
							
							printf("MOVSBL %d(%s), %s\n", value, register2, register1);
							i+=5;
							memaddr+=5;
							break;
				
				
				
				default:
						break;
							
				
			}
			
		}
		
		
	
	return 0;
	
}

int main (int argc, char **argv) {

	if (argc > 2) {
		fprintf(stderr, "ERROR: Too many arguments.\n");
		return 1;
	}
	else if (argc < 2) {
		fprintf(stderr, "ERROR: Too few arguments.\n");
		return 1;
	}
	
	if (strcmp(argv[1], "-h") == 0) {
		printf("Usage: y86dis <Y86 Filename>\n");
		return 0;
	}
	else {
		char * input = getInstructions(argv[1]);
		disassemble(input);
	}
	
	return 0;
}