/******************************************
*                                         
* Official Name:  Megan Jeanette Charters         
*                                         
* Nickname:  Megan      
*                                    
* E-mail:  mjcharte@syr.edu               
*                                         
* Assignment:  Final Project
*                                         
* Environment/Compiler:  Visual Studio Premium 2010               
*                                         
* Date:  December 4, 2012               
*                                         
*******************************************/
//Texture Credits
//desertsky.bmp thanks www.leeabbamonte.com
//skymountain.bmp thanks en.pudn.com
//sunset.bmp thanks en.pudn.com
// grass.bmp, thanks www.amazingtextures.com.
// nightsky.bmp, thanks anonymous.
// sky.bmp, thanks www.mega-tex.nl.
//cherrywood.bmp thanks Professor Baruch
//smallgrass.bmp thanks Professor Baruch

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <time.h>
#include < GL/glut.h > 
#include < GL/gl.h >
#include <string.h>
#ifdef __APPLE__
#  include <GLUT/glut.h>
#  include <OpenGL/glext.h>
#else
#  include <GL/glut.h>
#  include <GL/glext.h>
#endif
using namespace std;
const int XSIZE = 14; const int YSIZE = 11;
const int particleCount = 5000;
static int width, height;
static int askOrder = 1;
static int cubeLength = 2;
static float recoil = 5;
static float mazeX = -5; static float mazeZ = -9;
static float cubeCenter= mazeX+1;
static float cubeHalf = 1;
static float collideMargin = .55625;
static float walkamount = .125; static float walkreverse = .04;
static float rotateamount = .08;
float xpos = 0; float ypos = 0; //initial starting coordinates
float xrot = 276.88; //rotation of camera
bool topDown = false; bool timedMode1 = false;
static int lives = 3;
static int objects = 0; //objects collected
static int objectCollide = 0; //if collide with an object
static int order = 1; //level
static int maxobjects; //set at start screen when player picks level
static char message[10];
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.
static unsigned int texture[7]; // Array of texture indices.
static unsigned long startTime = 0; static unsigned long endTime = 0; static unsigned long currentTime = 0;
static unsigned long previousTime = 0;static int amountTime = 60; //in sec
static int timesPlayed = 0; static int wonGame = false;
static float density = .05; //[Swiftless]
static float fogColor[4] = {.7, .7, .7, 0}; //[Swiftless]
// Control points for the bezier curve
static float controlPoints4[6][4][3] = {
	{{-3.0, 1.75, 2.0}, {-3.0, 0.0, -2.0}, {-3.0, 0.0, -2.0}, {-3.0, 1.75, 2.0}},
	{{-2.0, 2.75, 2.0}, {-0.25, 0.0, -2.0}, {0.25, 0.0, -2.0}, {1.0, 2.75, 2.0}},
	{{-2.5, 2.75, 2.0}, {-0.25, 0.0, 2.0}, {0.25, 0.0, 2.0}, {1.5, 2.75, 2.0}},
	{{-2.5, 2.75, 1.0}, {-0.25, 0.0, -1.0}, {0.25, 0.0, -1.0}, {1.5, 2.75, 1.0}},
	{{-2.0, 2.75, 2.0}, {-0.25, 0.0, -2.0}, {0.25, 0.0, -2.0}, {1.0, 2.75, 2.0}},
	{{-1.0, 1.75, 3.0}, {-1.0, 0.0, -3.0}, {-1.0, 0.0, -3.0}, {-1.0, 1.75, 3.0}}
};
struct Particle { //[Heinz]
	float red; float green; float blue; //[Heinz]
	float xcoord; float ycoord; float zcoord; //[Heinz]
	float xspeed, yspeed, zspeed; //[Heinz]
	float accel; float deccel; float direction; //[Heinz]
}; //[Heinz]
Particle water[particleCount]; //[Heinz]
void createWater() { //[Heinz]
	for(int i = 0; i< particleCount; i++) { //[Heinz]
		water[i].red = .4; water[i].green = .4; water[i].blue = .8; //[Heinz]
		water[i].xcoord = (rand() % (22 - 18 + 1)) + 18; water[i].ycoord = -1;water[i].zcoord = (rand() % (18-14+1)) + 18; //[Heinz]
		water[i].xspeed = (((((rand()%500) + 1) * rand()%500) + 1) * 0.0003) - (((((rand()%50) + 1)* rand()%50) + 1) * 0.00005); //[Heinz]
		water[i].zspeed = (((((rand()%50) + 1) * rand()%500) + 1) * 0.00008) - (((((rand()%50) + 10)* rand()%50) + 1) * 0.0005);  //[Heinz]
		water[i].direction = 0; //[Heinz]
		water[i].accel = ((((5 * rand()%5) + 10)* rand()%50) + 7) * 0.02; //[Heinz]
		water[i].deccel = 0.0025; //[Heinz]
	} //[Heinz]
} //[Heinz]
void updateWater() { //[Heinz]
	for (int i = 1; i < particleCount; i++) { //[Heinz]
glColor3f(water[i].red, water[i].green, water[i].blue); //[Heinz]
water[i].ycoord += water[i].accel - water[i].deccel; //[Heinz]
water[i].deccel += 0.0025; //[Heinz]
water[i].xcoord += water[i].xspeed; //[Heinz]
water[i].zcoord += water[i].zspeed; //[Heinz]
water[i].direction += ((((((int)(0.5 - 0.1 + 0.1) * rand()%21) + 1) - 1 + 1) * rand()%21) + 1); //[Heinz]
if (water[i].ycoord < -1){ //[Heinz]
water[i].xcoord = 20; //[Heinz]
water[i].ycoord = -1; //[Heinz]
water[i].zcoord = 16; //[Heinz]
water[i].red = 1; //[Heinz]
water[i].green = 1; //[Heinz]
water[i].blue = 1; //[Heinz]
water[i].direction = 0; //[Heinz]
water[i].accel = ((((((8 - 5 + 2) * rand()%11) + 5) - 1 + 1) * rand()%11) + 1) * 0.02; //[Heinz]
water[i].deccel = 0.0025; //[Heinz]
} //[Heinz]
} //[Heinz]
} //[Heinz]
void drawWater1() { //[Heinz]
	for(int i = 0; i< particleCount; i++) { //[Heinz]
		glPushMatrix(); //[Heinz]
    glTranslatef (water[i].xcoord, water[i].ycoord, water[i].zcoord); //[Heinz]
	 glRotatef (water[i].direction - 90, 0, 0, 1); //[Heinz]
	glScalef(.125, .125, .125); //[Heinz]
    glBegin (GL_QUADS); //[Heinz]
    glTexCoord2d (0, 0);    glVertex3f (-1, -1, 0); //[Heinz]
    glTexCoord2d (1, 0);    glVertex3f (1, -1, 0); //[Heinz]
    glTexCoord2d (1, 1);    glVertex3f (1, 1, 0); //[Heinz]
    glTexCoord2d (0, 1);    glVertex3f (-1, 1, 0); //[Heinz]
    glEnd(); //[Heinz]
	glPopMatrix(); //[Heinz]
	} //[Heinz]
} //[Heinz]
struct Vector {
	float x; float y; float z;
};
struct BoundingBox { //[Reierstad]
	Vector max; //[Reierstad]
	Vector min; //[Reierstad]
	int collideTimes; int mazex; int mazey; 
}; //[Reierstad]
BoundingBox objectArray[2]; //[Reierstad]
BoundingBox objectArray2[3]; //[Reierstad]
BoundingBox objectArray3[4]; //[Reierstad]
BoundingBox objectArray4[4]; //[Reierstad]
BoundingBox entranceMaze1; //[Reierstad]
BoundingBox exitMaze1; //maze1[XSIZE-1][YSIZE] //[Reierstad]
BoundingBox maze11[XSIZE][YSIZE]; //[Reierstad]
BoundingBox maze21[XSIZE][YSIZE]; //[Reierstad]
BoundingBox maze31[XSIZE][YSIZE]; //[Reierstad]
BoundingBox maze41[XSIZE][YSIZE]; //[Reierstad]
struct BitMapFile {
   int sizeX;
   int sizeY;
   unsigned char *data;
};
BitMapFile *getBMPData(string filename) {
   BitMapFile *bmp = new BitMapFile;
   unsigned int size, offset, headerSize;
   // Read input file name.
   ifstream infile(filename.c_str(), ios::binary);
   // Get the starting point of the image data.
   infile.seekg(10);
   infile.read((char *) &offset, 4); 
   // Get the header size of the bitmap.
   infile.read((char *) &headerSize,4);
   // Get width and height values in the bitmap header.
   infile.seekg(18);
   infile.read( (char *) &bmp->sizeX, 4);
   infile.read( (char *) &bmp->sizeY, 4);
   // Allocate buffer for the image.
   size = bmp->sizeX * bmp->sizeY * 24;
   bmp->data = new unsigned char[size];
   // Read bitmap data.
   infile.seekg(offset);
   infile.read((char *) bmp->data , size);
   // Reverse color from bgr to rgb.
   int temp;
   for (int i = 0; i < size; i += 3) { 
      temp = bmp->data[i];
	  bmp->data[i] = bmp->data[i+2];
	  bmp->data[i+2] = temp;
   }
   return bmp;
}
void loadExternalTextures() {
   // Local storage for bmp image data.
   BitMapFile *image[7];
   // Load the textures.
   image[0] = getBMPData("TEXTURES/sunset.bmp");
   image[1] = getBMPData("TEXTURES/sky.bmp");
   image[2] = getBMPData("TEXTURES/bricks.bmp");
   image[3] = getBMPData("TEXTURES/nightSky.bmp");
   image[4] = getBMPData("TEXTURES/grass.bmp");
   image[5] = getBMPData("TEXTURES/cherrywood.bmp");
   image[6] = getBMPData("TEXTURES/smallgrass.bmp");
   // Bind grass image to texture index[0]. 
   glBindTexture(GL_TEXTURE_2D, texture[0]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[0]->sizeX, image[0]->sizeY, 0, 
	            GL_RGB, GL_UNSIGNED_BYTE, image[0]->data);
   // Bind sky image to texture index[1]
   glBindTexture(GL_TEXTURE_2D, texture[1]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[1]->sizeX, image[1]->sizeY, 0, 
	            GL_RGB, GL_UNSIGNED_BYTE, image[1]->data);
   // Bind sky image to texture index[1]
   glBindTexture(GL_TEXTURE_2D, texture[2]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[2]->sizeX, image[2]->sizeY, 0, 
	            GL_RGB, GL_UNSIGNED_BYTE, image[2]->data);
   // Bind sky image to texture index[1]
   glBindTexture(GL_TEXTURE_2D, texture[3]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[3]->sizeX, image[3]->sizeY, 0, 
	            GL_RGB, GL_UNSIGNED_BYTE, image[3]->data);
   // Bind sky image to texture index[1]
   glBindTexture(GL_TEXTURE_2D, texture[4]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[4]->sizeX, image[4]->sizeY, 0, 
	            GL_RGB, GL_UNSIGNED_BYTE, image[4]->data);
   // Bind sky image to texture index[1]
   glBindTexture(GL_TEXTURE_2D, texture[5]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[5]->sizeX, image[5]->sizeY, 0, 
	            GL_RGB, GL_UNSIGNED_BYTE, image[5]->data);
    // Bind sky image to texture index[1]
   glBindTexture(GL_TEXTURE_2D, texture[6]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[6]->sizeX, image[6]->sizeY, 0, 
	            GL_RGB, GL_UNSIGNED_BYTE, image[6]->data);
}
class Cube {
public:
	Cube(float sizeCube, int hasItem1, bool finish1);
	void draw();
	float getSize() { return size;}; //0 if not solid
	int hasItem() { return item;};
	bool isDone() { return finish;};
	void noItem() { item = 2;};
	void setItem(int number3) { item = number3;};
private:
	float size; int item; bool finish;
};
Cube::Cube(float sizeCube, int hasItem1, bool finish1) {
	size = sizeCube/2;
	item = hasItem1;
	finish = finish1;
}
void writeBitmapString(void *font, char *string){  
   char *c;
   for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}
Cube maze1[XSIZE][YSIZE] = 
{Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0),
Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,1,0), Cube(2,0,0), Cube(0,0,0), Cube(0,0,0), Cube(2,0,0),
Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(2,0,0),
Cube(2,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,1,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(2,0,0), Cube(0,0,0), Cube(0,0,0), Cube(2,0,0),
Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0),
Cube(2,0,0), Cube(2,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(0,0,0), Cube(0,0,0), Cube(2,0,0),
Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(2,0,0),
Cube(2,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(2,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0),
Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(2,0,0),
Cube(2,0,0), Cube(0,0,0), Cube(0,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(2,0,0),
Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(0,0,0), Cube(0,0,0), Cube(2,0,0),
Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0),
Cube(2,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(2,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,1),
Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0),	Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0)
}; //maze 1
Cube maze2[XSIZE][YSIZE] = 
{Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0),
Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,1,0), Cube(2,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0),
Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(2,0,0),
Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(0,1,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(2,0,0), Cube(2,0,0),
Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0),
Cube(2,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(2,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(0,0,0), Cube(0,0,0), Cube(2,0,0),
Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(2,0,0),
Cube(2,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(2,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0),
Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(2,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(2,0,0),
Cube(2,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(2,0,0),
Cube(2,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(0,0,0), Cube(0,0,0), Cube(2,0,0), Cube(2,0,0), Cube(0,0,0), Cube(0,0,0), Cube(2,0,0),
Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(0,0,0), Cube(0,0,0), Cube(2,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0),
Cube(2,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,1,0), Cube(2,0,0), Cube(0,0,0), Cube(0,0,1),
Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0),	Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0)
}; //maze 2
Cube maze3[XSIZE][YSIZE] = 
{Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0),
Cube(2,0,0), Cube(0,1,0), Cube(2,0,0), Cube(0,1,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(2,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0),
Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(2,0,0),
Cube(2,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(2,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0),
Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(2,0,0),
Cube(2,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0),
Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(2,0,0),
Cube(2,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0),
Cube(2,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(0,0,0), Cube(0,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0),
Cube(2,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0),
Cube(2,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0),
Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(0,1,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0),
Cube(2,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,1,0), Cube(0,0,0), Cube(0,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(0,0,0), Cube(0,0,1),
Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0),	Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0)
}; //maze 3
Cube maze4[XSIZE][YSIZE] = 
{Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0),
Cube(2,0,0), Cube(0,1,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(2,0,0), Cube(2,0,0), Cube(0,1,0), Cube(2,0,0),
Cube(2,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(2,0,0),
Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0),
Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(2,0,0),
Cube(2,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0),
Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(2,0,0),
Cube(2,0,0), Cube(2,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0),
Cube(2,0,0), Cube(0,0,0), Cube(0,0,0), Cube(2,0,0), Cube(0,0,0), Cube(0,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0),
Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0),
Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0),
Cube(2,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0), Cube(0,0,0), Cube(2,0,0),
Cube(2,0,0), Cube(0,1,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(0,0,0), Cube(2,0,0), Cube(0,1,0), Cube(2,0,0), Cube(0,0,0), Cube(0,0,1),
Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0),	Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0), Cube(2,0,0)
}; //maze 4
void camera() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if(topDown == true) {
		gluLookAt(-2, 30, 5,   7,-2,5,   0, 1, 0);
	}
	else gluLookAt(xpos,0,ypos,  xpos+cos(xrot),0,ypos+sin(xrot),   0.0,1.0,0.0);
 }
void drawCube(float x, float y, float z) {
	//Top of cube
	glBegin(GL_QUADS);
	glTexCoord2d(1, 1);	glVertex3f(x+cubeHalf, cubeHalf, z-cubeHalf);
	glTexCoord2d(0, 1);	glVertex3f(x-cubeHalf, cubeHalf, z-cubeHalf);
	glTexCoord2d(0, 0);	glVertex3f(x-cubeHalf, cubeHalf, z+cubeHalf);
	glTexCoord2d(1, 0);	glVertex3f(x+cubeHalf, cubeHalf, z+cubeHalf);
	glEnd();
	//Bottom
	glBegin(GL_QUADS);
	glTexCoord2d(1, 1);	glVertex3f(x+cubeHalf, -cubeHalf, z+cubeHalf);
	glTexCoord2d(0, 1);	glVertex3f(x-cubeHalf, -cubeHalf, z+cubeHalf);
	glTexCoord2d(0, 0);	glVertex3f(x-cubeHalf, -cubeHalf, z-cubeHalf);
	glTexCoord2d(1, 0);	glVertex3f(x+cubeHalf, -cubeHalf, z-cubeHalf);
	glEnd();
	//Front
	glBegin(GL_QUADS);
	glTexCoord2d(1, 1);	glVertex3f(x+cubeHalf, cubeHalf, z+cubeHalf);
	glTexCoord2d(0, 1);	glVertex3f(x-cubeHalf, cubeHalf, z+cubeHalf);
	glTexCoord2d(0, 0);	glVertex3f(x-cubeHalf, -cubeHalf, z+cubeHalf);
	glTexCoord2d(1, 0);	glVertex3f(x+cubeHalf, -cubeHalf, z+cubeHalf);
	glEnd();
	//Back
	glBegin(GL_QUADS);
	glTexCoord2d(1, 1);	glVertex3f(x-cubeHalf, cubeHalf, z-cubeHalf);
	glTexCoord2d(0, 1);	glVertex3f(x+cubeHalf, cubeHalf, z-cubeHalf);
	glTexCoord2d(0, 0);	glVertex3f(x+cubeHalf, -cubeHalf, z-cubeHalf);
	glTexCoord2d(1, 0);	glVertex3f(x-cubeHalf, -cubeHalf, z-cubeHalf);
	glEnd();
	//Left
	glBegin(GL_QUADS);
	glTexCoord2d(1, 1); glVertex3f(x-cubeHalf, cubeHalf, z+cubeHalf);
	glTexCoord2d(0, 1); glVertex3f(x-cubeHalf, cubeHalf, z-cubeHalf);
	glTexCoord2d(0, 0);	glVertex3f(x-cubeHalf, -cubeHalf, z-cubeHalf);
	glTexCoord2d(1, 0);	glVertex3f(x-cubeHalf, -cubeHalf, z+cubeHalf);
	glEnd();
	//Right
	glBegin(GL_QUADS);
	glTexCoord2d(1, 1);	glVertex3f(x+cubeHalf, -cubeHalf, z-cubeHalf);
	glTexCoord2d(0, 1);	glVertex3f(x+cubeHalf, -cubeHalf, z+cubeHalf);
	glTexCoord2d(0, 0);	glVertex3f(x+cubeHalf, cubeHalf, z+cubeHalf);
	glTexCoord2d(1, 0);	glVertex3f(x+cubeHalf, cubeHalf, z-cubeHalf);
	glEnd();
}
void drawObject(float x, float y, float z, float radius) {
	glPushMatrix();
	glTranslatef(x, y, z);
	glutSolidCube(radius);
	glPopMatrix();
}
void drawBlend(float x, float y, float z, float radius) {
	glPushMatrix();
	glTranslatef(x, y, z);
	glEnable(GL_BLEND);
	
	glutSolidSphere(radius, 20, 20);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Specify blending parameters.
	glDepthMask(GL_FALSE);
	glColor4f(.5, 1, .8, .7);
	glPopMatrix();
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}
void drawBezier() {
    glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
    glEvalMesh2(GL_LINE, 0, 20, 0, 20);
}
void drawEntrance() {
	glColor3f(0, .5, .5);
	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 6, controlPoints4[0][0]);
	glEnable(GL_MAP2_VERTEX_3); // Enable the Bezier surface.
	glPushMatrix();
	glTranslatef(1, -1, -7);
	glScalef(1.5, .4, .5);
	drawBezier();
	glPopMatrix();
}
void placeObjects() {
	int number = 0;
 for(int y=0; y<YSIZE; y++) {
  for(int x=0; x<XSIZE ; x++) {
	  if(order == 1) {
	if(maze1[x][y].hasItem() == 1) {
			if(number == 0) {
				glColor4f(.7, .5, .4, .5);
				drawBlend(cubeCenter+(x*cubeLength),    0.0,    mazeZ+1+cubeLength+(y*cubeLength), .5);
			}
		   if(number==1){
			  glColor3f(1, 0, 0);
			drawObject(cubeCenter+(x*cubeLength),    0.0,    mazeZ+1+cubeLength+(y*cubeLength), .5);
			}
		if(objects == 0) {
		objectArray[number].max.x = cubeCenter+(x*cubeLength)+.5; //[Reierstad]
		objectArray[number].min.x = cubeCenter+(x*cubeLength)-.5; //[Reierstad]
		objectArray[number].max.z = mazeZ+1+cubeLength+(y*cubeLength)+.5; //[Reierstad]
		objectArray[number].min.z = mazeZ+1+cubeLength+(y*cubeLength)-.5; //[Reierstad]
		objectArray[number].mazex = x;
		objectArray[number].mazey = y;
		objectArray[number].collideTimes = 0;
		number++;
		}
	} //end if for maze1
	  }
  } //end for
 } //end for
} //end placeObjects
void placeObjects2() {
	int number = 0;
 for(int y=0; y<YSIZE; y++) {
  for(int x=0; x<XSIZE ; x++) {
	if(maze2[x][y].hasItem() == 1) {
			if(number == 0) {
				glColor4f(4, .5, .8, .5);
				drawBlend(cubeCenter+(x*cubeLength),    0.0,    mazeZ+1+cubeLength+(y*cubeLength), .5);
			}
		   if(number==1){
			  glColor3f(1, 0, 0);
			drawObject(cubeCenter+(x*cubeLength),    0.0,    mazeZ+1+cubeLength+(y*cubeLength), .5);
			}
		  else if(number == 2) {
			  glColor4f(1, .5, 0, .5);
			  drawObject(cubeCenter+(x*cubeLength),    0.0,    mazeZ+1+cubeLength+(y*cubeLength), .5);
		  }
		if(objects == 0) {
		objectArray2[number].max.x = cubeCenter+(x*cubeLength)+.5; //[Reierstad]
		objectArray2[number].min.x = cubeCenter+(x*cubeLength)-.5; //[Reierstad]
		objectArray2[number].max.z = mazeZ+1+cubeLength+(y*cubeLength)+.5; //[Reierstad]
		objectArray2[number].min.z = mazeZ+1+cubeLength+(y*cubeLength)-.5; //[Reierstad]
		objectArray2[number].mazex = x;
		objectArray2[number].mazey = y;
		objectArray2[number].collideTimes = 0;
		number++;
		} //end if objects == 0
	} //end if for maze2
  } //end for
 } //end for
}
void placeObjects3() {
	int number = 0;
 for(int y=0; y<YSIZE; y++) {
  for(int x=0; x<XSIZE ; x++) {
	if(maze3[x][y].hasItem() == 1) {
			if(number == 0) {
				glColor4f(1, .5, .8, .5);
				drawBlend(cubeCenter+(x*cubeLength),    0.0,    mazeZ+1+cubeLength+(y*cubeLength), .5);
			}
		   if(number==1){
			  glColor3f(1, 0, 0);
			  drawObject(cubeCenter+(x*cubeLength),    0.0,    mazeZ+1+cubeLength+(y*cubeLength), .5);
			}
		  else if(number == 2) {
			  glColor4f(1, .5, 0, .5);
			  drawObject(cubeCenter+(x*cubeLength),    0.0,    mazeZ+1+cubeLength+(y*cubeLength), .5);
		  }
		  else if(number == 3) {
			  glColor4f(.3, .7, .2, .5);
				drawBlend(cubeCenter+(x*cubeLength),    0.0,    mazeZ+1+cubeLength+(y*cubeLength), .5);
		  }
		if(objects == 0) {
		objectArray3[number].max.x = cubeCenter+(x*cubeLength)+.5; //[Reierstad]
		objectArray3[number].min.x = cubeCenter+(x*cubeLength)-.5; //[Reierstad]
		objectArray3[number].max.z = mazeZ+1+cubeLength+(y*cubeLength)+.5; //[Reierstad]
		objectArray3[number].min.z = mazeZ+1+cubeLength+(y*cubeLength)-.5; //[Reierstad]
		objectArray3[number].mazex = x;
		objectArray3[number].mazey = y;
		objectArray3[number].collideTimes = 0;
		number++;
		} //end if objects == 0
	} //end if for maze3
  } //end for
 } //end for
}
void placeObjects4() {
	int number = 0;
 for(int y=0; y<YSIZE; y++) {
  for(int x=0; x<XSIZE ; x++) {
	if(maze4[x][y].hasItem() == 1) {
			if(number == 0) {
				glColor4f(1, .5, .8, .5);
				drawObject(cubeCenter+(x*cubeLength),    0.0,    mazeZ+1+cubeLength+(y*cubeLength), .5);
			}
		   if(number==1){
			  glColor3f(1, 0, 0);
			  drawBlend(cubeCenter+(x*cubeLength),    0.0,    mazeZ+1+cubeLength+(y*cubeLength), .5);
			}
		  else if(number == 2) {
			  glColor4f(1, .5, 0, .5);
			  drawObject(cubeCenter+(x*cubeLength),    0.0,    mazeZ+1+cubeLength+(y*cubeLength), .5);
		  }
		  else if(number == 3) {
			  glColor4f(.3, .7, .2, .5);
				drawBlend(cubeCenter+(x*cubeLength),    0.0,    mazeZ+1+cubeLength+(y*cubeLength), .5);
		  }
		if(objects == 0) {
		objectArray4[number].max.x = cubeCenter+(x*cubeLength)+.5; //[Reierstad]
		objectArray4[number].min.x = cubeCenter+(x*cubeLength)-.5; //[Reierstad]
		objectArray4[number].max.z = mazeZ+1+cubeLength+(y*cubeLength)+.5; //[Reierstad]
		objectArray4[number].min.z = mazeZ+1+cubeLength+(y*cubeLength)-.5; //[Reierstad]
		objectArray4[number].mazex = x;
		objectArray4[number].mazey = y;
		objectArray4[number].collideTimes = 0;
		number++;
		} //end if objects == 0
	} //end if for maze3
  } //end for
 } //end for
}
bool checkExitCollision() {
	if(xpos < (cubeCenter+((XSIZE-2)*cubeLength))+1+collideMargin && xpos > (cubeCenter+((XSIZE-2)*cubeLength))-1-collideMargin) { //[Reierstad]
		if(ypos < (mazeZ+1+cubeLength+(YSIZE*cubeLength))+1+collideMargin && ypos > (mazeZ+1+cubeLength+(YSIZE*cubeLength))-1-collideMargin) { //[Reierstad]
			return 1;
		}
	}
	return 0;
}
bool checkEntranceCollision() { //for maze 1, 2
	if(xpos < (cubeCenter+((XSIZE-(XSIZE-1))*cubeLength))+1+collideMargin && xpos > (cubeCenter+((XSIZE-(XSIZE-1))*cubeLength))-1-collideMargin) { //[Reierstad]
		if(ypos < (mazeZ+1+cubeLength+((YSIZE-YSIZE-1)*cubeLength))+1+collideMargin && ypos > (mazeZ+1+cubeLength+((YSIZE-YSIZE-1)*cubeLength))-1-collideMargin) { //[Reierstad]
			return 1;
		}
	}
	return 0;
}
bool checkBoundingBox() {
	for(int n = 0; n < maxobjects; n++) {
		if(order == 1) {
	if(xpos < objectArray[n].max.x && xpos > objectArray[n].min.x) { //[Reierstad]
		if(ypos < objectArray[n].max.z && ypos > objectArray[n].min.z) { //[Reierstad]
			objectCollide = n;
			return 1;
		}
	}
	}
	if(order == 2) {
	if(xpos < objectArray2[n].max.x && xpos > objectArray2[n].min.x) { //[Reierstad]
		if(ypos < objectArray2[n].max.z && ypos > objectArray2[n].min.z) { //[Reierstad]
			objectCollide = n;
			return 1;
		}
	}
	}
	if(order == 3) {
	if(xpos < objectArray3[n].max.x && xpos > objectArray3[n].min.x) { //[Reierstad]
		if(ypos < objectArray3[n].max.z && ypos > objectArray3[n].min.z) { //[Reierstad]
			objectCollide = n;
			return 1;
		}
	}
	}
	if(order == 4) {
	if(xpos < objectArray4[n].max.x && xpos > objectArray4[n].min.x) { //[Reierstad]
		if(ypos < objectArray4[n].max.z && ypos > objectArray4[n].min.z) { //[Reierstad]
			objectCollide = n;
			return 1;
		}
	}
	}
	}
	return 0;
}
void setTime() {
	if(order == 1) amountTime = 90;
	if(order == 2 || order == 3) amountTime = 180;
	if(order == 4) amountTime = 240;
   startTime = clock();
   previousTime = startTime;
   endTime = startTime + (amountTime * 1000);
}
bool collide() {
	for(int y=0; y<YSIZE; y++){
		 for(int x=0; x<XSIZE ; x++){ //check maze number
			 if(order == 1) {
			   if((maze1[x][y].getSize() > 0)) {
				   if(xpos <= maze11[x][y].max.x && xpos >= maze11[x][y].min.x) {
					if(ypos <= maze11[x][y].max.z && ypos >= maze11[x][y].min.z) {
						return 1;
					}
					}
				  }
				}
			if(order == 2) {
				if((maze2[x][y].getSize() > 0)) {
				   if(xpos <= maze21[x][y].max.x && xpos >= maze21[x][y].min.x) {
					if(ypos <= maze21[x][y].max.z && ypos >= maze21[x][y].min.z) {
					 return 1;
					}
					}
				}
			}
			if(order == 3) {
				if((maze3[x][y].getSize() > 0)) {
				   if(xpos <= maze31[x][y].max.x && xpos >= maze31[x][y].min.x) {
					if(ypos <= maze31[x][y].max.z && ypos >= maze31[x][y].min.z) {
					 return 1;
					}
					}
				}
			}
			if(order == 4) {
				if((maze4[x][y].getSize() > 0)) {
				   if(xpos <= maze41[x][y].max.x && xpos >= maze41[x][y].min.x) {
					if(ypos <= maze41[x][y].max.z && ypos >= maze41[x][y].min.z) {
					 return 1;
					}
					}
				}
			}
		}
	}
	return 0; 
}
void setArrays1() {
	maze1[objectArray[objectCollide].mazex][objectArray[objectCollide].mazey].noItem();
	  objectArray[objectCollide].collideTimes += 1;
	  if (objectArray[objectCollide].collideTimes == 1) {
		  if(lives < 3 && lives > 0) lives += 1;
	  switch(objects) {
	  case 0: objects = 1;
		  break;
	  case 1: objects = 2;
		  break;
	  case 2: if(maxobjects == 2) objects = 2; 
			  else objects = 3;
			  break;
	  case 3:
		  if(maxobjects == 3) objects = 3;
		  else objects = 4;
		  break;
	  case 4: 
		  if(maxobjects == 4) objects = 4;
		  else objects = 4;
		  break;
	  }
	  }
	  glutPostRedisplay();
}
void setArrays2() {
	maze2[objectArray2[objectCollide].mazex][objectArray2[objectCollide].mazey].noItem();
	  objectArray2[objectCollide].collideTimes += 1;
	  if (objectArray2[objectCollide].collideTimes == 1) {
		  if(lives < 3 && lives > 0) lives += 1;
	  switch(objects) {
	  case 0: objects = 1;
		  break;
	  case 1: objects = 2;
		  break;
	  case 2: if(maxobjects == 2) objects = 2; 
			  else objects = 3;
			  break;
	  case 3:
		  if(maxobjects == 3) objects = 3;
		  else objects = 4;
		  break;
	  case 4: 
		  if(maxobjects == 4) objects = 4;
		  else objects = 4;
		  break;
	  }
	  }
	  glutPostRedisplay();
}
void setArrays3() {
	maze3[objectArray3[objectCollide].mazex][objectArray3[objectCollide].mazey].noItem();
	  objectArray3[objectCollide].collideTimes += 1;
	  if (objectArray3[objectCollide].collideTimes == 1) {
		  if(lives < 3 && lives > 0) lives += 1;
	  switch(objects) {
	  case 0: objects = 1;
		  break;
	  case 1: objects = 2;
		  break;
	  case 2: if(maxobjects == 2) objects = 2; 
			  else objects = 3;
			  break;
	  case 3:
		  if(maxobjects == 3) objects = 3;
		  else objects = 4;
		  break;
	  case 4: 
		  if(maxobjects == 4) objects = 4;
		  else objects = 4;
		  break;
	  }
	  }
	  glutPostRedisplay();
}
void setArrays4() {
	maze4[objectArray4[objectCollide].mazex][objectArray4[objectCollide].mazey].noItem();
	  objectArray4[objectCollide].collideTimes += 1;
	  if (objectArray4[objectCollide].collideTimes == 1) {
		  if(lives < 3 && lives > 0) lives += 1;
	  switch(objects) {
	  case 0: objects = 1;
		  break;
	  case 1: objects = 2;
		  break;
	  case 2: if(maxobjects == 2) objects = 2; 
			  else objects = 3;
			  break;
	  case 3:
		  if(maxobjects == 3) objects = 3;
		  else objects = 4;
		  break;
	  case 4: 
		  if(maxobjects == 4) objects = 4;
		  else objects = 4;
		  break;
	  }
	  }
	  glutPostRedisplay();
}
void move(float amount) {
  xpos+=cos(xrot)*amount;
  ypos+=sin(xrot)*amount;
  if(order == 1 || order == 2) {
  if(checkEntranceCollision()) {
	  entranceMaze1.collideTimes += 1;
	  if(objects < maxobjects) {		  
	  if(entranceMaze1.collideTimes >= 1) {
		  if(lives > 1) {
			  xpos-=recoil*2*cos(xrot)*amount;
			   ypos-=recoil*2*sin(xrot)*amount;
			 lives -= 1;
		  }
		  else if(lives <= 1) {
			  timesPlayed += 1;
			  wonGame = false;
			  askOrder = 1; 
			  order = 1;
			  glutPostRedisplay();
			  }
		  }
	}
  }
  }
  if(checkExitCollision()) {
	  exitMaze1.collideTimes += 1;
	  if(askOrder == 2) {
		  if(currentTime <= endTime) {
			askOrder = 1;
			order = 1;
			timesPlayed += 1;
			wonGame = true;
			glutPostRedisplay();
		  }
		  else if((currentTime-previousTime) >= (endTime-previousTime)) {
			askOrder = 1;
			order = 1;
			timesPlayed += 1;
			wonGame = false;
			glutPostRedisplay();
		} 
	  }
	  if(objects == maxobjects ) {
		  askOrder = 1;
		  order = 1;
		  timesPlayed += 1;
  		  wonGame = true;
		  glutPostRedisplay();
	  }
	  if(objects < maxobjects) {		  
	  if(exitMaze1.collideTimes >= 1) {
		  if(lives > 1) {
			  xpos-=recoil*2*cos(xrot)*amount;
			   ypos-=recoil*2*sin(xrot)*amount;
			 lives -= 1;
		  }
		  else if(lives == 1) {
			  timesPlayed += 1;
			  wonGame = false;
			  askOrder = 1; 
			  order = 1;
			  glutPostRedisplay();
		  }
	  }
	  }
  }
  if(checkBoundingBox()) { //check maze num
	  if(order == 1) setArrays1();
	  else if(order == 2) setArrays2();
	  else if(order == 3) setArrays3();
	  else if(order == 4) setArrays4();
  }
  if(collide()) {
   xpos-=recoil*cos(xrot)*amount;
   ypos-=recoil*sin(xrot)*amount;
  } 
  if(collide()) {
   xpos+=recoil*cos(xrot)*amount;
   ypos+=recoil*sin(xrot)*amount;
   xpos-=cos(xrot)*amount;
   ypos-=sin(xrot)*amount; 
  }
 }
void floatToString(char * destStr, int precision, float val) {
   sprintf(destStr,"%f",val);
   destStr[precision] = '\0';
}
void drawObjects(int number) {
	floatToString(message, 1, (float)number);
	glRasterPos2f(.29, .95);
	writeBitmapString((void*)font, message);
	glRasterPos2f(.31, .95);
	writeBitmapString((void*)font, "/");
	floatToString(message, 1, (float)maxobjects);
	glRasterPos2f(.33, .95);
	writeBitmapString((void*)font, message);
}
void drawLives(int number) {
	floatToString(message, 1, (float)number);
	glRasterPos2f(.1, .95);
	writeBitmapString((void*)font, message);
}
void drawTimer(int number) {
	int precise; float value1 = amountTime - (float)number;
	if(value1 < 10) precise = 1;
	else if(value1 < 100) precise = 2;
	else precise = 3;
	floatToString(message, precise, value1);
	glRasterPos2f(.25, .95);
	writeBitmapString((void*)font, message);
}
void onScreenHelp() {
	if(askOrder == 2) {
		glRasterPos2f(.02,.95); //top left corner
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "Time Remaining: ");
		drawTimer((currentTime-previousTime)/CLOCKS_PER_SEC);
	}
	if(askOrder == 3) {
		 glRasterPos2f(0.02,0.95); //top left corner
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "Lives: ");
		drawLives(lives);
		 glRasterPos2f(0.2,0.95); //top left corner
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "Found: ");
		drawObjects(objects);
	}
	glutPostRedisplay();
}
void printHelp() {
	glMatrixMode(GL_PROJECTION);
	 glLoadIdentity(); 
	 gluOrtho2D(0,1,0,1);
	 glMatrixMode(GL_MODELVIEW);
	 glLoadIdentity();
	 glColor3f(0, 0, 0);
	 glRasterPos2f(0.02,0.95); //top left corner
	 glDisable(GL_LIGHTING);
	 onScreenHelp();
	 glEnable(GL_LIGHTING);
	 glMatrixMode(GL_PROJECTION); 
	 glLoadIdentity(); 
  	 gluPerspective(45, (double)width / (double)height, 1, 100);
	 glMatrixMode(GL_MODELVIEW); 
	 glLoadIdentity();
}
void drawMaze1() {
 for(int y=0; y<YSIZE; ++y ) {
  for(int x=0; x<XSIZE ; ++x ) {
   if((maze1[x][y].getSize()>0)){
    drawCube(cubeCenter+(x*cubeLength),   0, mazeZ+cubeLength+1+(y*cubeLength)); 
	maze11[x][y].max.x = (cubeCenter+(x*cubeLength))+cubeHalf+collideMargin;
	maze11[x][y].min.x = (cubeCenter+(x*cubeLength))-cubeHalf-collideMargin;
	maze11[x][y].max.z = (mazeZ+cubeLength+1+(y*cubeLength))+cubeHalf+collideMargin;
	maze11[x][y].min.z = (mazeZ+cubeLength+1+(y*cubeLength))-cubeHalf-collideMargin;
   }
  }
 }
}
void drawMaze2() {
 for(int y=0; y<YSIZE; ++y ) {
  for(int x=0; x<XSIZE ; ++x ) {
   if((maze2[x][y].getSize()>0)){
    drawCube(cubeCenter+(x*cubeLength),   0, mazeZ+cubeLength+1+(y*cubeLength)); 
	maze21[x][y].max.x = (cubeCenter+(x*cubeLength))+cubeHalf+collideMargin;
	maze21[x][y].min.x = (cubeCenter+(x*cubeLength))-cubeHalf-collideMargin;
	maze21[x][y].max.z = (mazeZ+cubeLength+1+(y*cubeLength))+cubeHalf+collideMargin;
	maze21[x][y].min.z = (mazeZ+cubeLength+1+(y*cubeLength))-cubeHalf-collideMargin;
   }
  }
 }
}
void drawMaze3() {
 for(int y=0; y<YSIZE; ++y ) {
  for(int x=0; x<XSIZE ; ++x ) {
   if((maze3[x][y].getSize()>0)){
    drawCube(cubeCenter+(x*cubeLength),   0, mazeZ+cubeLength+1+(y*cubeLength)); 
	maze31[x][y].max.x = (cubeCenter+(x*cubeLength))+cubeHalf+collideMargin;
	maze31[x][y].min.x = (cubeCenter+(x*cubeLength))-cubeHalf-collideMargin;
	maze31[x][y].max.z = (mazeZ+cubeLength+1+(y*cubeLength))+cubeHalf+collideMargin;
	maze31[x][y].min.z = (mazeZ+cubeLength+1+(y*cubeLength))-cubeHalf-collideMargin;
   }
  }
 }
}
void drawMaze4() {
 for(int y=0; y<YSIZE; ++y ) {
  for(int x=0; x<XSIZE ; ++x ) {
   if((maze4[x][y].getSize()>0)){
    drawCube(cubeCenter+(x*cubeLength),   0, mazeZ+cubeLength+1+(y*cubeLength)); 
	maze41[x][y].max.x = (cubeCenter+(x*cubeLength))+cubeHalf+collideMargin;
	maze41[x][y].min.x = (cubeCenter+(x*cubeLength))-cubeHalf-collideMargin;
	maze41[x][y].max.z = (mazeZ+cubeLength+1+(y*cubeLength))+cubeHalf+collideMargin;
	maze41[x][y].min.z = (mazeZ+cubeLength+1+(y*cubeLength))-cubeHalf-collideMargin;
   }
  }
 }
}
void skyandground() {
 glBindTexture(GL_TEXTURE_2D,texture[0]); //ground
 glBegin(GL_QUADS); 
 glTexCoord2d(1.0,1.0); glVertex3f( width/30, height/30,-25); 
 glTexCoord2d(0.0,1.0); glVertex3f( -width/30, height/30,-25); 
 glTexCoord2d(0.0,0.0); glVertex3f( -width/30, -height/30,-25); 
 glTexCoord2d(1.0,0.0); glVertex3f( width/30, -height/30,-25); 
 glEnd();
}
void ground() {
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBegin(GL_QUADS); 
	glTexCoord2d(.40,.40); glVertex3f( width/30, height/30,-35); 
	glTexCoord2d(0.0,.40); glVertex3f( -width/30, height/30,-35); 
	glTexCoord2d(0.0,0.0); glVertex3f( -width/30, -height/30,-35); 
	glTexCoord2d(.40,0.0); glVertex3f( width/30, -height/30,-35); 
	glEnd();
}
void mazeBlend() {
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ZERO); // Specify blending parameters to overwrite background.
	if(order == 1) {
	   glBindTexture(GL_TEXTURE_2D, texture[1]); 
	   glDepthMask(GL_FALSE);
	   drawMaze1();
   }
	else if(order == 2) {
		glBindTexture(GL_TEXTURE_2D, texture[5]); 
		glDepthMask(GL_FALSE);
		drawMaze2();
	}
	else if(order == 3) {
		glBindTexture(GL_TEXTURE_2D, texture[1]); 
		glDepthMask(GL_FALSE);
		drawMaze3();
	}
	else if(order == 4) {
		glBindTexture(GL_TEXTURE_2D, texture[3]); 
		glDepthMask(GL_FALSE);
		drawMaze4();
	}
   glColor4f(1.0, 1.0, 1.0, .2);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Specify blending parameters to mix skies.       
   if(order == 1) {
	   glBindTexture(GL_TEXTURE_2D, texture[2]); 
	   drawMaze1();
   }
	else if(order == 2) {
		glBindTexture(GL_TEXTURE_2D, texture[2]); 
		drawMaze2();
	}
	else if(order == 3) {
		glBindTexture(GL_TEXTURE_2D, texture[2]); 
		drawMaze3();
	}
	else if(order == 4) {
		glBindTexture(GL_TEXTURE_2D, texture[2]); 
		drawMaze4();
	}
   glDepthMask(GL_TRUE);
   glDisable(GL_BLEND);
}
void timedMode() { //take in int what level, askOrder = 2
	currentTime = clock();
	glFogi(GL_FOG_MODE, GL_EXP); //[Swiftless]
	glFogfv(GL_FOG_COLOR, fogColor); //[Swiftless]
	glFogf(GL_FOG_DENSITY, density); //[Swiftless]
	glHint(GL_FOG_HINT, GL_NICEST); //[Swiftless]
	printHelp();
	glEnable(GL_TEXTURE_2D);
	skyandground();
	camera();
	glEnable(GL_FOG); //[Swiftless]
	mazeBlend();
	glEnable(GL_DEPTH_TEST);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); 
	glDisable(GL_TEXTURE_2D);
	updateWater(); //[Heinz]
	drawWater1(); //[Heinz]
}
void drawRest(int number1) {
	printHelp();
	glFogi(GL_FOG_MODE, GL_EXP); //[Swiftless]
	glFogfv(GL_FOG_COLOR, fogColor); //[Swiftless]
	glFogf(GL_FOG_DENSITY, density); //[Swiftless]
	glHint(GL_FOG_HINT, GL_NICEST); //[Swiftless]
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	if(topDown == true) {
		ground();
	}
	else skyandground();
	camera();
	glEnable(GL_FOG); //[Swiftless]
	if(topDown == true)	{
		glColor4f(.4, 0, 1, 1); 
		glDisable(GL_FOG); 
		drawBlend(xpos, 0, ypos, .5);
		glEnable(GL_FOG); 
	}
	if(objects < maxobjects) {
		glDisable(GL_TEXTURE_2D);
		updateWater(); //[Heinz]
		drawWater1(); //[Heinz]
		drawEntrance();
		glEnable(GL_TEXTURE_2D);
    }
	if(number1 == 1) {
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	drawMaze1();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_FOG); 
	placeObjects();
	glEnable(GL_FOG); 
	}
	if(number1 == 2) {
	glBindTexture(GL_TEXTURE_2D, texture[6]);
	drawMaze2();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_FOG);
	placeObjects2();
	glEnable(GL_FOG);
	}
	if(number1 == 3) {
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	drawMaze3();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_FOG);
	placeObjects3();
	glEnable(GL_FOG);
	}
	if(number1 == 4) {
	glBindTexture(GL_TEXTURE_2D, texture[5]);
	drawMaze4();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_FOG);
	placeObjects4();
	glEnable(GL_FOG);
	}
}
void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_COLOR_MATERIAL);
	if(askOrder == 1) { //start screen, pick level
		glMatrixMode(GL_PROJECTION);
	 glLoadIdentity(); 
	gluOrtho2D(0,1,0,1);
	 glMatrixMode(GL_MODELVIEW);
	 glLoadIdentity();
		if(timesPlayed > 0) {
			glColor3f(0.0, 0.0, 0.0);
		    glRasterPos3f(.4, .55, 0);
			if(wonGame == true) {
				writeBitmapString((void*)font, "You win!");
			}
			else {
				writeBitmapString((void*)font, "You lost. Try again.");
			}
		}
		glColor3f(0.0, 0.0, 0.0);
		glRasterPos3f(.5, .5, 0);
		writeBitmapString((void*)font, "Press Enter to Start");
  	    glRasterPos3f(.5, .45, 0.0);
        writeBitmapString((void*)font, "Level: ");
	    glColor3f(1.0, 0.0, 0.0);
	  glRasterPos3f(.6, .45, 0.0);
	  switch (order) {
	     case 1:
	        writeBitmapString((void*)font, "1");
			lives = 3;
			order = 1;
			objects = 0;
			maxobjects = 2;
			xpos = -2; ypos = -5;
			xrot = 276.88;
		    break;
		 case 2:
			writeBitmapString((void*)font, "2");
			order = 2;
			lives = 3;
			maxobjects = 3;
			xpos = -2; ypos = -5;
			xrot = 276.88;
		    break;
		 case 3:
			writeBitmapString((void*)font, "3");
			order = 3;
			lives = 3;
			xpos = 10; ypos = 3;
			maxobjects = 4;
			xrot = 276.88;
		    break;
		 case 4:
			 writeBitmapString((void*)font, "4");
			order = 4;
			lives = 3;
			xpos = 10; ypos = 3;
			maxobjects = 4;
			xrot = 276.88;
			 break;
		 default:
	        break;
	  }
	  glColor3f(0, 0, 0);
	  glRasterPos3f(.5, .4, 0.0);
        writeBitmapString((void*)font, "Timed: ");
	    glColor3f(1.0, 0.0, 0.0);
	  glRasterPos3f(.6, .4, 0.0);
	  if(timedMode1 == true)
		  writeBitmapString((void*)font, "Yes");
	  else writeBitmapString((void*)font, "No");
	}
	else if(askOrder == 2) { //timed mode
		timedMode();
	}
	else { //askOrder == 3, display maze
		drawRest(order);
	}
	glutSwapBuffers();
}
void resetItems() {
	for(int y = 0; y < YSIZE; y++) {
			  for(int x = 0; x < XSIZE; x++) {
				  if(maze1[x][y].hasItem() == 2) {
					  maze1[x][y].setItem(1);
				  }
			  }
		  }
		  for(int y = 0; y < YSIZE; y++) {
			  for(int x = 0; x < XSIZE; x++) {
				  if(maze2[x][y].hasItem() == 2) {
					  maze2[x][y].setItem(1);
				  }
			  }
		  }
		  for(int y = 0; y < YSIZE; y++) {
			  for(int x = 0; x < XSIZE; x++) {
				  if(maze3[x][y].hasItem() == 2) {
					  maze3[x][y].setItem(1);
				  }
			  }
		  }
		  for(int y = 0; y < YSIZE; y++) {
			  for(int x = 0; x < XSIZE; x++) {
				  if(maze4[x][y].hasItem() == 2) {
					  maze4[x][y].setItem(1);
				  }
			  }
		  }
}
void keyInput(unsigned char key, int x, int y){
   switch (key) {
      case 27:
         exit(0);
         break;
	  case 'a':
		  if(askOrder == 3) {
		  topDown = !topDown; 
		  glutPostRedisplay();
		  }
		  break;
	  case '1':	
         askOrder = 1;
		 objects = 0;
		 order = 1;
	     glutPostRedisplay();
         break;
      case 13:	
		  if(timedMode1 == true) {
			  askOrder = 2;
			  setTime();
		  }
		  else {
			  askOrder = 3;
		  objects = 0;
		  resetItems();
		  }
		 topDown = false;
	     glutPostRedisplay();
         break;
      default:
         break;
   }
}
void specialKeyInput(int key, int x, int y) {
 if(key == GLUT_KEY_UP) {
	 if(askOrder == 1) {
		if(order < 4) order+=1;
	 }
	 else {
		 if(topDown == false)
	   move(walkamount);
	 }
 }
 if(key == GLUT_KEY_DOWN) {
	 if(askOrder == 1) {
		if(order > 1) order-=1;
	 }
	 else {
		 if(topDown == false)
		move(-walkamount);  
	 }
 }
 if(key == GLUT_KEY_RIGHT) {
	 if(askOrder == 1) {
		 if(timedMode1 == false) {
			 timedMode1 = true;
		 }
	 }
	 else {
		 if(topDown == false)
		 xrot+=rotateamount;
	 }
 }
 if(key == GLUT_KEY_LEFT) {
	 if(askOrder == 1) {
		 if(timedMode1 == true) timedMode1 = false;
	 }
	 else {
		 if(topDown == false)
			xrot-=rotateamount;
	 }
 }
 glutPostRedisplay();
}
void setup(void) {
   glClearColor(1, 1, 1, 0);
   glEnable(GL_DEPTH_TEST);
   createWater(); //[Heinz]
   glGenTextures(7, texture);
   loadExternalTextures();
   glEnable(GL_TEXTURE_2D); 
   srand(time(0));
}
void resize(int w, int h) {
   glViewport (0, 0, (GLsizei)w, (GLsizei)h);
   glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
    gluPerspective(45, (double)w / (double)h, 1, 100);
  glMatrixMode(GL_MODELVIEW);
   width = w;
   height = h;
}
void printInteraction(void) {
   cout << "Interaction:" << endl;
   cout << "Press enter to start." << endl;
   cout << "Press up and down arrow keys to change level of maze." << endl;
   cout << "Press left and right arrow keys to toggle timed mode." << endl;
   cout << "Press 'a' to toggle top down view in maze." << endl;
   cout << "Press up and down arrow keys to move forward and backwards in maze." << endl;
   cout << "Press left and right arrow keys to rotate the camera (look left and right)." << endl;
   cout << "Press 1 to go back to start screen." << endl;
}
int main(int argc, char **argv) {
   printInteraction();
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("Maze.cpp");
   setup(); 
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);
   glutMainLoop(); 
   return 0;  
}

/* [Heinz] Heinz. "Tutorial 8 - Particle Systems". www.naturewizard.com. */
/* [Reierstad] Reierstad, Ronny André. "Bounding Box Collision Test". www.morrowland.com. */
/* [Swiftless] Swiftless. "OpenGL Fog". www.swiftless.com. */
