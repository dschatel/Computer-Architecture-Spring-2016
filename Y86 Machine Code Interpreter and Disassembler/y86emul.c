#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "y86emul.h"

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

int run_program(Y86 * program) {
	
	int reg_one;
	int reg_two;
	
	int value;
	int return_val;
	int run_prog = 1;
	char input;
	
	
	while (run_prog == 1) {
		
		//printf("Current Instruction: %x\n", program->memory[program->counter]);
		switch(program->memory[program->counter]) {
			
			case 0x00:	//NOP Instruction
						program->counter++;
						break;
			case 0x10:	//HLT Instruction
						printf(HLT);
						printf(AOK);
						run_prog = 0;
						return 0;
						break;
			case 0x20:	//rrmovl rA, rB
			
						reg_one = (program->memory[program->counter+1] & 0xf0) >> 4;
						reg_two = (program->memory[program->counter+1] & 0x0f);
						
						program->y86reg[reg_two] = program->y86reg[reg_one];
						
						program->counter+=2;
						break;
			case 0x30: 	//irmovl, V,rB
						
						reg_two = (program->memory[program->counter+1] & 0x0f);
						
												
						value = (program->memory[program->counter+5] << 24) +
								(program->memory[program->counter+4] << 16) +
								(program->memory[program->counter+3] << 8) +
								(program->memory[program->counter+2]);
								
						//printf("rB: %d, imm: %d\n", reg_two, value);
								
						program->y86reg[reg_two] = value;
						program->counter+=6;
						break;
			case 0x40:	//rmmovl, rA,D(rB)
						
						reg_one = (program->memory[program->counter+1] & 0xf0) >> 4; //Obtain rA from register byte
						reg_two = (program->memory[program->counter+1] & 0x0f); // Obtain rB from register byte
						
						value = 	(program->memory[program->counter+5] << 24) +
											(program->memory[program->counter+4] << 16) +
											(program->memory[program->counter+3] << 8) +
											(program->memory[program->counter+2]); //obtain displacement from 4 byte value
											
						
						//Place 4 byte signed int from register into 4 bytes in memory					
						program->memory[program->y86reg[reg_two] + value] = (program->y86reg[reg_one] & 0x000000ff);
						program->memory[program->y86reg[reg_two] + value + 1] = (program->y86reg[reg_one] & 0x0000ff00) >> 8;
						program->memory[program->y86reg[reg_two] + value + 2] = (program->y86reg[reg_one] & 0x00ff0000) >> 16;
						program->memory[program->y86reg[reg_two] + value + 3] = (program->y86reg[reg_one] & 0xff000000) >> 24;
						
						program->counter += 6;
						break;
			case 0x50: //mrmovl D(rB), rA
					
						reg_one = (program->memory[program->counter+1] & 0xf0) >> 4; //Obtain rA from register byte
						reg_two = (program->memory[program->counter+1] & 0x0f); // Obtain rB from register byte
						
						value = 	(program->memory[program->counter+5] << 24) +
											(program->memory[program->counter+4] << 16) +
											(program->memory[program->counter+3] << 8) +
											(program->memory[program->counter+2]); //obtain displacement from 4 byte value
											
						program->y86reg[reg_one] = 	(program->memory[program->y86reg[reg_two] + value]) +
													(program->memory[program->y86reg[reg_two] + value + 1] << 8) +
													(program->memory[program->y86reg[reg_two] + value + 2] << 16)+ 
													(program->memory[program->y86reg[reg_two] + value + 3] << 24);
													
						program->counter+=6;
						break;
			case 0x60: //addl rA, rB
			
						reg_one = (program->memory[program->counter+1] & 0xf0) >> 4; //Obtain rA from register byte
						reg_two = (program->memory[program->counter+1] & 0x0f); // Obtain rB from register byte
						
						program->zero_flag = 0;
						program->overflow_flag = 0;
						program->negative_flag = 0;
						
						value = program->y86reg[reg_two] + program->y86reg[reg_one];
						
						if (value == 0)
							program->zero_flag = 1;
						
						if ((program->y86reg[reg_one] > 0 && program->y86reg[reg_two] > 0 && value < 0) || (program->y86reg[reg_one] < 0 && program->y86reg[reg_two] < 0 && value > 0))
							program->overflow_flag = 1;
						
						if (value < 0)
							program->negative_flag = 1;
						
						program->y86reg[reg_two] = value;
						
						//printf("Register %d is now %d\n", reg_two, program->y86reg[reg_two]);
						//printf("Addl -- ZF: %d, OF: %d, SF: %d\n", program->zero_flag, program->overflow_flag, program->negative_flag);
						
						program->counter+=2;
						break;
			case 0x61:	//subl, rA, rB
			
						reg_one = (program->memory[program->counter+1] & 0xf0) >> 4; //Obtain rA from register byte
						reg_two = (program->memory[program->counter+1] & 0x0f); // Obtain rB from register byte
						
						program->zero_flag = 0;
						program->overflow_flag = 0;
						program->negative_flag = 0;
						
						value = program->y86reg[reg_two] - program->y86reg[reg_one];
						
						if (value == 0)
							program->zero_flag = 1;
						
						if ((program->y86reg[reg_one] < 0 && program->y86reg[reg_two] > 0 && value < 0) || (program->y86reg[reg_one] > 0 && program->y86reg[reg_two] < 0 && value > 0))
							program->overflow_flag = 1;
						
						if (value < 0)
							program->negative_flag = 1;
						
						program->y86reg[reg_two] = value;
						
						//printf("Register %d is now %d\n", reg_two, program->y86reg[reg_two]);
						//printf("Subl -- ZF: %d, OF: %d, SF: %d\n", program->zero_flag, program->overflow_flag, program->negative_flag);
						
						program->counter+=2;
						break;
			case 0x62: //andl, rA, rB
			
						reg_one = (program->memory[program->counter+1] & 0xf0) >> 4; //Obtain rA from register byte
						reg_two = (program->memory[program->counter+1] & 0x0f); // Obtain rB from register byte
						
						program->zero_flag = 0;
						program->negative_flag = 0;
						
						value = program->y86reg[reg_two] & program->y86reg[reg_one];
						
						if (value < 0)
							program->negative_flag = 1;
						
						if (value == 0)
							program->zero_flag = 1;
						
						program->counter+=2;
						break;
			case 0x63: //xorl rA, rB
						
						reg_one = (program->memory[program->counter+1] & 0xf0) >> 4; //Obtain rA from register byte
						reg_two = (program->memory[program->counter+1] & 0x0f); // Obtain rB from register byte
						
						program->zero_flag = 0;
						program->negative_flag = 0;
						
						value = program->y86reg[reg_two] ^ program->y86reg[reg_one];
						
						if (value < 0)
							program->negative_flag = 1;
						
						if (value == 0)
							program->zero_flag = 1;
						
						program->counter+=2;
						break;
			case 0x64: //mull rA, rB
						reg_one = (program->memory[program->counter+1] & 0xf0) >> 4; //Obtain rA from register byte
						reg_two = (program->memory[program->counter+1] & 0x0f); // Obtain rB from register byte
						
						program->zero_flag = 0;
						program->overflow_flag = 0;
						program->negative_flag = 0;
						
						value = program->y86reg[reg_two] * program->y86reg[reg_one];
						
						if (value == 0)
							program->zero_flag = 1;
						
						if (	(program->y86reg[reg_one] > 0 && program->y86reg[reg_two] > 0 && value < 0) || 
								(program->y86reg[reg_one] < 0 && program->y86reg[reg_two] < 0 && value < 0) ||
								(program->y86reg[reg_one] < 0 && program->y86reg[reg_two] > 0 && value > 0) ||
								(program->y86reg[reg_one] > 0 && program->y86reg[reg_two] < 0 && value > 0))
							program->overflow_flag = 1;
						
						if (value < 0)
							program->negative_flag = 1;
						
						program->y86reg[reg_two] = value;
						
						program->counter+=2;
						break;
			case 0x65: //cmpl rA, rB

						reg_one = (program->memory[program->counter+1] & 0xf0) >> 4; //Obtain rA from register byte
						reg_two = (program->memory[program->counter+1] & 0x0f); // Obtain rB from register byte
						
						program->zero_flag = 0;
						program->overflow_flag = 0;
						program->negative_flag = 0;
						
						value = program->y86reg[reg_two] - program->y86reg[reg_one];
						
						if (value == 0)
							program->zero_flag = 1;
						
						if ((program->y86reg[reg_one] < 0 && program->y86reg[reg_two] > 0 && value < 0) || (program->y86reg[reg_one] > 0 && program->y86reg[reg_two] < 0 && value > 0))
							program->overflow_flag = 1;
						
						if (value < 0)
							program->negative_flag = 1;
						
						program->counter+=2;
						break;
						
			case 0x70: //jmp dst
			
					value = 	(program->memory[program->counter + 4] << 24) +
										(program->memory[program->counter + 3] << 16) +
										(program->memory[program->counter + 2] << 8) +
										(program->memory[program->counter + 1]);
					
					program->counter = value;
					break;
					
			case 0x71: //jle dst
						
					value = 	(program->memory[program->counter + 4] << 24) +
										(program->memory[program->counter + 3] << 16) +
										(program->memory[program->counter + 2] << 8) +
										(program->memory[program->counter + 1]);
					
					if (((program->negative_flag ^ program->overflow_flag) | program->zero_flag) == 1)
						program->counter = value;
					else
						program->counter+=5;
					break;
					
			case 0x72: //jl dst
			
					value = 	(program->memory[program->counter + 4] << 24) +
										(program->memory[program->counter + 3] << 16) +
										(program->memory[program->counter + 2] << 8) +
										(program->memory[program->counter + 1]);
					
					if ((program->negative_flag ^ program->overflow_flag) == 1)
						program->counter = value;
					else
						program->counter+=5;
					break;
					
			case 0x73: //je dst
			
					value			 = 	(program->memory[program->counter + 4] << 24) +
										(program->memory[program->counter + 3] << 16) +
										(program->memory[program->counter + 2] << 8) +
										(program->memory[program->counter + 1]);
										
					//printf("Jump if equal -- ZF: %d, Go to address %d\n", program->zero_flag, value);
					
					if (program->zero_flag == 1)
						program->counter = value;
					else
						program->counter+=5;
					break;
			
			case 0x74: //jne dst
				
										
					value			 = 	(program->memory[program->counter + 4] << 24) +
										(program->memory[program->counter + 3] << 16) +
										(program->memory[program->counter + 2] << 8) +
										(program->memory[program->counter + 1]);
										
					//printf("Jump if not equal -- ZF: %d, Go to address %d\n", program->zero_flag, value);
					
					if (program->zero_flag == 0)
						program->counter = value;
					else
						program->counter+=5;
					break;
					
			case 0x75: //jge dst
			
					value = 			(program->memory[program->counter + 4] << 24) +
										(program->memory[program->counter + 3] << 16) +
										(program->memory[program->counter + 2] << 8) +
										(program->memory[program->counter + 1]);
					
					if (~(program->negative_flag ^ program->overflow_flag) == 1)
						program->counter = value;
					else
						program->counter+=5;
					break;
					
			case 0x76: //jg dst
			
					value			 = 	(program->memory[program->counter + 4] << 24) +
										(program->memory[program->counter + 3] << 16) +
										(program->memory[program->counter + 2] << 8) +
										(program->memory[program->counter + 1]);
					
					if ((~(program->negative_flag ^ program->overflow_flag) & ~(program->zero_flag)) == 1)
						program->counter = value;
					else
						program->counter+=5;
					break;
			
			case 0x80: //call dst
			
					value			 = 	(program->memory[program->counter + 4] << 24) +
										(program->memory[program->counter + 3] << 16) +
										(program->memory[program->counter + 2] << 8) +
										(program->memory[program->counter + 1]); //Get memory address for called function
					
					//printf("Move to mem address: %d, current Instruction Address is: %d\n", value, program->counter);
					
					return_val = program->counter + 5;
					//printf("Return value: %d\n", return_val);
					//printf("Current top of stack: %d\n", program->y86reg[ESP]);
					program->y86reg[ESP] -=4; //Make room on call stack
					//printf("Making room on stack for return address: %d\n", program->y86reg[ESP]);
				
					//Store 4-byte memory address of instruction sent to upon return
					program->memory[program->y86reg[ESP]] = (return_val & 0x000000ff);
					program->memory[program->y86reg[ESP]+1] = (return_val & 0x0000ff00) >> 8;
					program->memory[program->y86reg[ESP]+2] = (return_val & 0x00ff0000) >> 16;
					program->memory[program->y86reg[ESP]+3] = (return_val & 0xff000000) >> 24;
				
					program->counter = value; //Send program to new instruction
					
					break;
			
			case 0x90: //ret
					
					//printf("Current top of stack: %d\n", program->y86reg[ESP]);
					
					value = (program->memory[program->y86reg[ESP]]) + 
							(program->memory[program->y86reg[ESP]+1] << 8) +
							(program->memory[program->y86reg[ESP]+2] << 16) +
							(program->memory[program->y86reg[ESP]+3] << 24);
							
					//printf("Return address: %d\n", value);
										
					program->counter = value;
				
					program->y86reg[ESP] += 4;
					break;
			
			case 0xa0: //pushl rA

					reg_one = (program->memory[program->counter+1] & 0xf0) >> 4;
				
					program->y86reg[ESP] -= 4;
				
					program->memory[program->y86reg[ESP]] = (program->y86reg[reg_one] & 0x000000ff);
					program->memory[program->y86reg[ESP]+1] = (program->y86reg[reg_one] & 0x0000ff00) >> 8;
					program->memory[program->y86reg[ESP]+2] = (program->y86reg[reg_one] & 0x00ff0000) >> 16;
					program->memory[program->y86reg[ESP]+3] = (program->y86reg[reg_one] & 0xff000000) >> 24;
					
					
					program->counter += 2;
					break;
			
			case 0xb0: //popl rA
			
					reg_one = (program->memory[program->counter+1] & 0xf0) >> 4;
					
					value = (program->memory[program->y86reg[ESP]]) + 
							(program->memory[program->y86reg[ESP]+1] << 8) +
							(program->memory[program->y86reg[ESP]+2] << 16) +
							(program->memory[program->y86reg[ESP]+3] << 24);
							
					program->y86reg[reg_one] = value;
					
					program->y86reg[ESP]+=4;
					program->counter += 2;
					break;
			
			case 0xc0: //readb D(rA)
			
					reg_one = (program->memory[program->counter+1] & 0xf0) >> 4;
					
					value = 	(program->memory[program->counter+5] << 24) +
								(program->memory[program->counter+4] << 16) +
								(program->memory[program->counter+3] << 8) +
								(program->memory[program->counter+2]); //obtain displacement from 4 byte value
					
					if (scanf("%c", &input) == EOF)
						program->zero_flag = 1;
					else
						program->zero_flag = 0;
					
					//printf("After readb, ZF is %d\n", program->zero_flag);
					program->memory[program->y86reg[reg_two] + value] = input;
					
					program->counter+=6;
					break;
			
			case 0xc1: //readl D(rA)
			
					reg_one = (program->memory[program->counter+1] & 0xf0) >> 4;
					
					value = 	(program->memory[program->counter+5] << 24) +
								(program->memory[program->counter+4] << 16) +
								(program->memory[program->counter+3] << 8) +
								(program->memory[program->counter+2]); //obtain displacement from 4 byte value
					
					if (EOF == scanf("%i", &return_val))
						program->zero_flag = 1;
					else
						program->zero_flag = 0;
					
					//printf("After readl, ZF is %d\n", program->zero_flag);
					
					program->memory[program->y86reg[reg_one] + value] = return_val & 0x000000ff;
					program->memory[program->y86reg[reg_one] + value + 1] = (return_val & 0x0000ff00) >> 8;
					program->memory[program->y86reg[reg_one] + value + 2] = (return_val & 0x00ff0000) >> 16;
					program->memory[program->y86reg[reg_one] + value + 3] = (return_val & 0xff000000) >> 24;
					
					program->counter+=6;
					break;
			
			case 0xd0: //writeb D(rA)
					
					reg_one = (program->memory[program->counter+1] & 0xf0) >> 4;
					
					value = 	(program->memory[program->counter+5] << 24) +
								(program->memory[program->counter+4] << 16) +
								(program->memory[program->counter+3] << 8) +
								(program->memory[program->counter+2]); //obtain displacement from 4 byte value
								
					//printf("Mem address %d with Displacement %d\n", program->y86reg[reg_one], value);
					
					printf("%c", program->memory[program->y86reg[reg_one] + value]);
					program->counter+=6;
					break;
			
			case 0xd1: //writel D(rA)
					
					reg_one = (program->memory[program->counter+1] & 0xf0) >> 4;
					
					value = 	(program->memory[program->counter+5] << 24) +
								(program->memory[program->counter+4] << 16) +
								(program->memory[program->counter+3] << 8) +
								(program->memory[program->counter+2]); //obtain displacement from 4 byte value
								
					//printf("Mem address %d with Displacement %d\n", program->y86reg[reg_one], value);
					
					return_val = 	(program->memory[program->y86reg[reg_one] + value]) +
									(program->memory[program->y86reg[reg_one] + value + 1] << 8) +
									(program->memory[program->y86reg[reg_one] + value + 2] << 16) +
									(program->memory[program->y86reg[reg_one] + value + 3] << 24);
					
					printf("%i", return_val);
					program->counter+=6;
					break;
			
			case 0xe0: //movsbl D(rB), rA
			
						reg_one = (program->memory[program->counter+1] & 0xf0) >> 4; //Obtain rA from register byte
						reg_two = (program->memory[program->counter+1] & 0x0f); // Obtain rB from register byte
						
						value = 			(program->memory[program->counter+5] << 24) +
											(program->memory[program->counter+4] << 16) +
											(program->memory[program->counter+3] << 8) +
											(program->memory[program->counter+2]); //obtain displacement from 4 byte value
						
						return_val = program->memory[program->y86reg[reg_two] + value];		
						return_val = return_val << 24;
						return_val = return_val >> 24;
						
						program->y86reg[reg_one] = return_val;
						
						//printf("Byte retrieved: %x\n", return_val);
						
				
				program->counter+=6;
				break;
			
			default:
			
				fprintf(stderr, INS);
				return 1;
				break;
		}
	}
				
	fprintf(stderr, ADR);
	return 1;
}				

char * openFile (char * filename) {
	
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

int setupY86 (char * input) {
	
	char * 	token = strtok(input, "\t\n");
	
	Y86 *chip = malloc(sizeof(Y86));
	
	chip->zero_flag = 0;
	chip->overflow_flag = 0;
	chip->negative_flag = 0;
	
	
	while(token != NULL) {
		//printf("Current token: %s\n", token);
			//printf("Counter: %d\n", chip->counter);
		
		if (strcmp(token, ".size") == 0) {
			token = strtok(NULL, "\t\n");
			//printf("Current token: %s\n", token);
			chip->mem_size = hex_to_dec(token);
			//printf("Memory Size: %d\n", chip->mem_size);
		
			chip->memory = malloc((chip->mem_size + 1) * sizeof(unsigned char));
		
			for(int i = 0; i < chip->mem_size; i++) {
				chip->memory[i] = 0;
			}
		
			for (int i = 0; i <= 8; i++) {
				chip->y86reg[i] = 0;
			}
			chip->counter = -1;
			token = strtok(NULL, "\t\n");
		
		}
		else if (strcmp(token, ".byte") == 0) {
			token = strtok(NULL, "\t\n");
			//printf("Mem Loc in Hex: %s\n", token);
			int loc = hex_to_dec(token);
			
			//printf("Mem Loc in Dec: %d\n", loc);
		
			token = strtok(NULL, "\t\n");
			unsigned char c = (unsigned char) hex_to_dec(token);
		
			//printf("Token as String: %s, As unsigned char: %c OR %x\n", token, c, c);
		
			chip->memory[loc] = c;
		
			//printf("Store Correctly? %x at %d\n", chip->memory[loc], loc);
			token = strtok(NULL, "\t\n");
		
		}
		else if(strcmp(token, ".long") == 0) {
			token = strtok(NULL, "\t\n");
			int loc = hex_to_dec(token);
			token = strtok(NULL, "\t\n");
			
			int num = atoi(token);
			
			*(unsigned int*)(chip->memory+loc) = num;
			
			//printf("%ld, %x + %x + %x+ %x\n", num, chip->memory[loc+3], chip->memory[loc+2], chip->memory[loc+1], chip->memory[loc]);
			token = strtok(NULL, "\t\n");
		}
		else if(strcmp(token, ".text") == 0) {
			token = strtok(NULL, "\t\n");
			int loc = hex_to_dec(token);
			token = strtok(NULL, "\t\n");
			
			//printf("Instruction Location Start: %d\n", loc);
			//printf("Counter: %d\n", chip->counter);
			
			if(chip->counter == -1)
				chip->counter = loc;
			else if (chip->counter != -1) {
				fprintf(stderr, INS);
				return 1;
			}
			
			//printf("Counter: %d\n", chip->counter);
			
			int i = loc;
			
			for (int j = 0; j < strlen(token); j+=2) {
				
				char * byte = malloc(sizeof(char) * 3);
				strncpy(byte, token + j, 2);
				byte[2] = '\0';
				
				//printf("Byte: %s\n", byte);
				
				unsigned char c = (unsigned char) hex_to_dec(byte);
				
				chip->memory[i] = c;
				//printf("This is in memory location %d: %x\n", i, chip->memory[i]);
				i++;
				
				
			}
			token = strtok(NULL, "\t\n");
			
		}
		else if (strcmp(token, ".string") == 0) {
			token = strtok(NULL, "\t\n");
			int loc = hex_to_dec(token);
			token = strtok(NULL, "\t\n");
			
			for (int i = 1; i < strlen(token); i++) {
				chip->memory[loc] = (unsigned char) token[i];
				loc++;
			}
			token = strtok(NULL, "\t\n");
		}
		else if (strcmp(token, ".bss") == 0) {
			token = strtok(NULL, "\t\n");
			int loc = hex_to_dec(token);
			token = strtok(NULL, "\t\n");
			
			for (int i = atoi(token); i > 0; i--) {
				chip->memory[loc] = (unsigned char) 0;
				loc++;
			}
			token = strtok(NULL, "\t\n");
		}
		else	{
			fprintf(stderr, INS);
			return 1;
		}
	
		
		
	}
	
	free(token);
	
	/*for(int i = 0; i < chip->mem_size; i++)
			printf("%x", chip->memory[i]);*/
		
	return run_program(chip);
	
	
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
		printf("Usage: y86emul <Y86 Filename>\n");
		return 0;
	}
	else {
		char * input = openFile(argv[1]);
		setupY86(input);
	}
	return 0;
}