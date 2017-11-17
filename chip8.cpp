#include <iostream>
#include <random>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include "chip8.h"

//display fontset for chip-8. Each character is represented by it's sprite.
//each sprite contains 5 hex codes to display a character.
unsigned char chip8_fontset[80] =				
{ 
    0xF0, 0x90, 0x90, 0x90, 0xF0, //0
    0x20, 0x60, 0x20, 0x20, 0x70, //1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
    0x90, 0x90, 0xF0, 0x10, 0x10, //4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
    0xF0, 0x10, 0x20, 0x40, 0x40, //7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
    0xF0, 0x90, 0xF0, 0x90, 0x90, //A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
    0xF0, 0x80, 0x80, 0x80, 0xF0, //C
    0xE0, 0x90, 0x90, 0x90, 0xE0, //D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
    0xF0, 0x80, 0xF0, 0x80, 0x80  //F
};

unsigned short buffer[512];				//use undefined	- maybe to load program to memory. Fill it using fopen in binary mode in loadGame function
unsigned short bufferSize;				//use undefined

unsigned short random_number() {
	
	std::random_device seeder;
  	std::ranlux48 gen(seeder());
	std::uniform_int_distribution<int>  uniform_0_255(0, 255);
	return (unsigned short)uniform_0_255(gen);
}

unsigned short * decimal_array(unsigned short) {
	
	unsigned short *ar = new unsigned short[3];
	return ar;
}

chip8::chip8() {
	//Constructor
	
}

void chip8::initialize() {
	
	pc     = 0x200;				//Program starts at 0x200
	opcode = 0;					//Current opcode = NULL
	I      = 0;					//Reset index register
	sp     = 0;					//Reset stack pointer	
	
	//Clear display
	for(int i=0; i<2048; ++i) {
		gfx[i] = 0;
	}
	
	//Clear stack
	for(int i=0; i<16; ++i) {
		stack[i] = 0;
	}
	
	//Clear registers
	for(int i=0; i<16; ++i) {
		V[i] = 0;
	}
	
	//Clear keyboard mapping
	for(int i=0; i<16; ++i) {
		key[i] = 0;
	}
	
	drawFlag = true;
	
	//Load first 80 memory locations with display information i.e.  0x000 - 0x050
	for(int i=0; i<80; ++i)	{
		memory[i+80] = chip8_fontset[i];		//font-set should be loaded from location 80 in memory
	}
	
	for(int i=0; i< bufferSize; ++i) {
		memory[i+512] = buffer[i];
	}
}

void chip8::emulateCycle() {
	
	//To increment pc, use pc +=2, not pc = pc + 2, as it is slower.
	//Populate opcode. It is 16-bit and thus OR two 'memory' locations
	opcode = memory[pc] << 8 | memory[pc+1];
	
	//Switch case to decode opcode
	switch(opcode & 0xF000) {				//AND with 0xF000 to make the opcode of 4 bytes
		
		case (0x0000):						//0x0nnn or 0x00E0 or 0x00EE
				switch (opcode & 0x000F) {
				
					case (0x0000):				//0x00E0
							for(int i=0; i<2048; ++i) {
								gfx[i] = 0;
							}
							drawFlag = true;
							pc = pc + 2;
							break;
						
					case (0x000E):				//0x00EE ( depricated )
							--sp;
							pc = stack[sp];
							pc = pc + 2;
							break;
						
					default:					//0x0nnn
							//complete section
							//Yet to implement this
							break;
				}
				break;
		
		case (0x1000):						//01nnn
				pc = (opcode & 0x0FFF);
				break;
		
		case (0x2000):						//0x2nnn
				stack[sp] = pc;
				++sp;
				pc = (opcode & 0x0FFF);
				break;
				
		case (0x3000):						//0x3xkk
				if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)) {
					pc = pc + 4;
				} else {
					pc = pc + 2;
				}
				break;
			
		case (0x4000):						//0x4xkk
				if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)) {
					pc = pc + 4;
				} else {
					pc = pc + 2;
				}
				break;
				
		case (0x5000):						//0x5xy0
				if(V[(opcode & 0x0F00) >> 9] == V[(opcode & 0x00F0)] >> 8) {
					pc = pc + 4;
				} else {
					pc = pc + 2;
				}
				break;
				
		case (0x6000):						//0x6xkk
				V[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF);
				pc = pc + 2;
				break;
				
		case (0x7000):						//0x7xkk
				V[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF);
				pc = pc + 2;
				break;
				
		case (0x8000):						
				switch(opcode & 0x000F) {
						case (0x0000):		//0x8xy0
								V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
								pc = pc + 2;
								break;
								
						case (0x0001):		//0x8xy1
								V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0) >> 4];
								pc = pc + 2;
								break;
								
						case (0x0002):		//0x8xy2
								V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4];
								pc = pc + 2;
								break;
								
						case (0x0003):		//0x8xy3
								V[(opcode & 0x0F00) >> 8] ^= V[(opcode & 0x00F0) >> 4];
								pc = pc + 2;
								break;
								
						case (0x0004):		//0x8xy4
								V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
								if (V[(opcode & 0x0F00)] > 255) {
									V[0x000F] = 1;
								} else {
									V[0x000F] = 0;
								}
								pc = pc + 2;
								break;
								
						case (0x0005):		//0x8xy5
								V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
								if(V[(opcode & 0x0F00) >> 8] > 255) {
									V[0x000F] = 1;
								}
								pc = pc + 2;
								break;
								
						case (0x0006):		//0x8xy6
								//Re-check this
								if((V[(opcode & 0x0F00) >> 8] & 0x0001) == 0x0001) {
									V[(opcode & 0x000F)] = 1;
								}
								V[(opcode & 0x0F00) >> 8] /=2;
								pc = pc + 2;
								break;
								
						case (0x0007):		//0x8xy7
								V[(opcode & 0x00F0) >> 4] -= V[(opcode & 0x0F00) >> 8];
								if(V[(opcode & 0x00F0) >> 4] > 255) {
									V[0x000F] = 1;
								} else {
									V[0x000F] = 0;
								}
								pc = pc + 2;
								break;
								
						case (0x000E):		//0x8xyE
								if((V[(opcode & 0x0F00) >> 8] & 0x0080) == 0x0001) {
									V[0x000F] = 1;
								}
								V[(opcode & 0x0F00) >> 8] *= 2;
								pc = pc + 2;
								break;
							
						default:
								std::cout<<"Unknown opcode: 0x%X\n"<<opcode<<std::endl;
								break;
				}
				break;
				
		case (0x9000):						//0x9xy0
				if(V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4]) {
					pc = pc + 4;
				} else {
					pc = pc + 2;
				}
				break;
		
		case (0xA000):						//0xAnnn
				I  = (opcode & 0x0FFF);
				pc = pc + 2;
				break;
		
		case (0xB000):						//0xBnnn
				stack[sp] = pc;
				++sp;
				pc = V[0] + (opcode & 0x0FFF);
				break;
				
		case (0xC000):						//0xCxnn
				//Code to generate random numbers
				//Creating a seperate function for Random Number because of "[Error] crosses initialization of 'std::ranlux48 gen'" or can use {} here.
				V[(opcode & 0x0F00) >> 8] = random_number() & (opcode & 0x00FF);
				pc = pc + 2;
				break;
		
		case (0xD000):						//0xDxyn
				//draw(Vx, Vy, N)			Draw a sprite at location Vx, Vy with height N and width 8 pixels. Read chip8_fonset from row I upto row I+N-1? 
				V[15] = 0;
				//A guessful implementation
				{
					unsigned short x = V[(opcode & 0x0F00) >> 8];
					unsigned short y = V[(opcode & 0x00F0) >> 4];
					unsigned short h = (opcode & 0x000F);
					unsigned short pixel;
					
					for(int i=0; i<h; ++i) {
						pixel = memory[I + i];
						for(int j=0; j<8; ++j) {
							//display logic
							//Yet to implement this
						}
					}
					drawFlag = true;
					pc = pc + 2;
				}
				break;
		
		case (0xE000):						//0xEx9e or 0xExa1
				switch (opcode & 0x000F) {
					
					case (0x000E):
							if(key[V[(opcode & 0x0F00) >> 8]] != 0) {
								pc = pc + 4;
							} else {
								pc = pc + 2;
							}
							break;
							
					case (0x0001):
							if(key[V[(opcode & 0x0F00) >> 8]] == 0) {
								pc = pc + 4;
							} else {
								pc = pc + 2;
							}
							break;
					
					default:
							std::cout<<"Unknown Opcode"<<std::endl;
							break;
				}
				break;
		
		case (0xF000):
				switch (opcode & 0x00FF) {
					
					case (0x0007):
							V[(opcode & 0x0F00) >> 8] = delay_timer;
							pc = pc + 2;
							break;
					
					case (0x000A):	
							//Check through all the keys if pressed
							{
								bool pressed = false;
								for(int i=0; i<16; ++i) {
									if(key[i] != 0) {
										V[(opcode & 0x0F00) >> 8] = i;
										pressed = true;
									}
								}
								
								if(!pressed) {
									return ;
								}
								
								pc = pc + 2;
							}
							break;
							
					case (0x0015):
							delay_timer = V[(opcode & 0x0F00) >> 8];
							pc = pc + 2;
							break;
							
					case (0x0018):
							sound_timer = V[(opcode & 0x0F00) >> 8];
							pc = pc + 2;
							break;
							
					case (0x001E):
							I += V[(opcode & 0x0F00) >> 8];
							pc = pc + 2;
							break;
							
					case (0x0029):
							I = V[(opcode & 0x0F00) >> 8] * 0x0005;
							pc = pc + 2;
							break;
							
					case (0x0033): {
							unsigned short value = V[(opcode & 0x0F00) >> 8];
							memory[I]   = (value / 100);
							memory[I+1] = (value / 10) % 10;
							memory[I+2] = (value % 100) % 10;
							}
							pc = pc + 2;
							break;
							
					case (0x0055):
							for(int i=0; i<=((opcode & 0x0F00) >> 8); ++i) {
								memory[I+i] = V[i];
							}
							I += ((opcode & 0x0F00) >> 8) + 1;
							pc = pc + 2;
							break;
							
					case (0x0065):
							for(int i=0; i<=((opcode & 0x0F00) >> 8); ++i) {
								V[i] = memory[I+i];
							}
							I += ((opcode & 0x0F00) >> 8) + 1;
							pc = pc + 2;
							break;
							
					default:
						std::cout<<"Unknown opcode: 0x%X\n"<<opcode<<std::endl;
						break;
				}
				break;
		
		default:
				std::cout<<"Unknown opcode: 0x%X\n"<<opcode<<std::endl;
				break;
	}
	
	//Update DT
	if(delay_timer > 0) {
		--delay_timer;
	}
	
	//Update ST
	if(sound_timer > 0) {
		if(sound_timer == 1) {
			std::cout<<"BEEP"<<std::endl;
		}
		--sound_timer;
	}
}

bool chip8::loadGame(const char *file_name) {
	
	//Call the initializer fucntion
	initialize();
	
	std::cout<<"Loading application file from disk..."<<std::endl;	
	
	//Open file in binary mode
	FILE *f = fopen(file_name, "rb");
	if(!f) {
		std::cout<<"Error in reading application file. Exiting..."<<std::endl;
		return false;
	}
	
	//check file size
	fseek(f, 0, SEEK_END);
	long lsize = ftell(f);
	rewind(f);
	std::cout<<"Filesize: "<<lsize<<std::endl;
	
	//allocate memroy to contain the whole file
	char *buffer = new char[lsize];
	if(!buffer) {
		std::cout<<"Memory error"<<std::endl;
		return false;
	}
	
	//copy file into the buffer
	size_t result = fread(buffer, 1, lsize, f);
	if(result != lsize) {
		std::cout<<"Reading error"<<std::endl;
		return false;
	}
	
	//copy buffer to memory
	unsigned short memory_left = 3584;		//4096 - 512, because first 512 is for memory
	if(memory_left > lsize) {
		for(int i=0; i<lsize; ++i) {
			memory[i+512] = buffer[i];
		}
	} else {
		std::cout<<"Program size greater than ROM"<<std::endl;
	}
	
	fclose(f);
	free(buffer);
	
	return true;
}
