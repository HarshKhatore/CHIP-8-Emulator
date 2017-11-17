#include <iostream>
#define GLUT_DISABLE_ATEXIT_HACK
#include <GL/glut.h>			//For OpenGL glut library. Why to put GL/glut.h?
#include "chip8.h"				//Custom project header file

chip8 myChip8;

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
//We can also use above 2 macros to define width and height by using a multiplier 
//example: display_height = SCREEN_HEIGHT * 10

int modifier = 10;
int display_width = 640;
int display_height = 320;

void display();
void reshape(GLsizei w, GLsizei h);
void keyboardUp(unsigned char key, int x, int y);
void keyboardDown(unsigned char key, int x, int y);


int main(int argc, char **argv) {
	
	//Set up graphics and Input
	//Uncomment below 2 lines after implementation
	//setupGraphics();
	//setupInput();
	
	if(argc < 2) {
		std::cout<<"Incorrect command line usage"<<std::endl;
		std::cout<<"Usage: myChip8.exe application_name"<<std::endl;
		return 1;
	}
	
	//Load game, if file not present, exit
	if(!myChip8.loadGame(argv[1])) {
		std::cout<<"No such application file present...Try again"<<std::endl;
		return 1;
	}
	
	//Initialize GLUT library
	glutInit(&argc, argv);
	
	//Set initial window position and dimension
	glutInitWindowSize(display_width, display_height);
	glutInitWindowPosition(320, 320);
	
	//Set initial display area
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	
	//Initial window 
	glutCreateWindow("Enjoy !!!");
	
	//Display function
	glutDisplayFunc(display);
	glutIdleFunc(display);
	
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboardDown);
	glutKeyboardUpFunc(keyboardUp);
	
	//Initialize the system and load game into memory
	myChip8.initialize();
	myChip8.loadGame("");
	
	//loop around for game
	//...
	
	return 0;
}
/* We added 3 glut files to the project liker option, then we must also add them to the linked command line option
 * as -lopenglu32 -lopenglu32 -lglu32
 */
 
// Old gfx code
void drawPixel(int x, int y)
{
	glBegin(GL_QUADS);
		glVertex3f((x * modifier) + 0.0f,     (y * modifier) + 0.0f,	 0.0f);
		glVertex3f((x * modifier) + 0.0f,     (y * modifier) + modifier, 0.0f);
		glVertex3f((x * modifier) + modifier, (y * modifier) + modifier, 0.0f);
		glVertex3f((x * modifier) + modifier, (y * modifier) + 0.0f,	 0.0f);
	glEnd();
}

void updateQuads(const chip8& chip) {
	
	for(int i=0; i<32; ++i) {
		for(int j=0; j<64; ++j) {
			if(chip.gfx[i*64 + j] == 0) {
				glColor3f(0.0f, 0.0f, 0.0f);
			} else {
				glColor3f(1.0f, 1.0f, 1.0f);
			}
			
			drawPixel(i, j);
		}
	}
}

void display() {
	
	//Call the primary display function
	myChip8.emulateCycle();
	
	if(myChip8.drawFlag) {
		
		//clear framebuffer	
		glClear(GL_COLOR_BUFFER_BIT);
		
		updateQuads(myChip8);
		
		//how is it used?
		glutSwapBuffers();
		
		myChip8.drawFlag = false;
	}
	
}

void keyboardDown(unsigned char key, int x, int y)
{
	if(key == 27)    // esc
		exit(0);

	if(key == '1') {
		myChip8.key[0x1] = 0;	
	} else if(key == '2') {
		myChip8.key[0x2] = 0;
	} else if(key == '3') {
		myChip8.key[0x3] = 0;
	} else if(key == '4') {
	 	myChip8.key[0xC] = 0;
	} else if(key == 'q') {
		myChip8.key[0x4] = 0;
	} else if(key == 'w') {
		myChip8.key[0x5] = 0;
	} else if(key == 'e') {
		myChip8.key[0x6] = 0;
	} else if(key == 'r') {
		myChip8.key[0xD] = 0;
	} else if(key == 'a') {
		myChip8.key[0x7] = 0;
	} else if(key == 's') {
		myChip8.key[0x8] = 0;
	} else if(key == 'd') {
		myChip8.key[0x9] = 0;
	} else if(key == 'f') {
		myChip8.key[0xE] = 0;
	} else if(key == 'z') {
		myChip8.key[0xA] = 0;
	} else if(key == 'x') {
		myChip8.key[0x0] = 0;
	} else if(key == 'c') {
		myChip8.key[0xB] = 0;
	} else if(key == 'v') {
		myChip8.key[0xF] = 0;
	}
}

void keyboardUp(unsigned char key, int x, int y)
{
	if(key == '1') {
		myChip8.key[0x1] = 0;	
	} else if(key == '2') {
		myChip8.key[0x2] = 0;
	} else if(key == '3') {
		myChip8.key[0x3] = 0;
	} else if(key == '4') {
	 	myChip8.key[0xC] = 0;
	} else if(key == 'q') {
		myChip8.key[0x4] = 0;
	} else if(key == 'w') {
		myChip8.key[0x5] = 0;
	} else if(key == 'e') {
		myChip8.key[0x6] = 0;
	} else if(key == 'r') {
		myChip8.key[0xD] = 0;
	} else if(key == 'a') {
		myChip8.key[0x7] = 0;
	} else if(key == 's') {
		myChip8.key[0x8] = 0;
	} else if(key == 'd') {
		myChip8.key[0x9] = 0;
	} else if(key == 'f') {
		myChip8.key[0xE] = 0;
	} else if(key == 'z') {
		myChip8.key[0xA] = 0;
	} else if(key == 'x') {
		myChip8.key[0x0] = 0;
	} else if(key == 'c') {
		myChip8.key[0xB] = 0;
	} else if(key == 'v') {
		myChip8.key[0xF] = 0;
	}
}
 
void reshape(GLsizei w, GLsizei h) {
	
	//specify clear values for the color buffers
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	
	//specify which matrix is the current matrix
	glMatrixMode(GL_PROJECTION);
	
	//replace the current matrix with the identity matrix
	glLoadIdentity();
	
	//a 2D orthographic projection matrix
	gluOrtho2D(0, w, h, 0);
	
	//specify which matrix is the current matrix
	glMatrixMode(GL_MODELVIEW);
	
	//set the viewport
	glViewport(0, 0, w,h);
	
	//Reset values
	display_width = w;
	display_height = h;
}
