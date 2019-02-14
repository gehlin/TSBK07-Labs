// Lab 1-1.
// This is the same as the first simple example in the course book,
// but with a few error checks.
// Remember to copy your file to a new on appropriate places during the lab so
// you keep old results. Note that the files "lab1-1.frag", "lab1-1.vert" are
// required.

// Should work as is on Linux and Mac. MS Windows needs GLEW or glee.
// See separate Visual Studio version of my demos.
#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include <math.h>
#include "loadobj.h"
#include "VectorUtils3.h"

#define PI 3.14
#define near 1.0
#define far 100.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5

GLfloat t;
GLuint program;
Model *mWalls;
Model *mRoof;
Model *mBalcony;
Model *mBladeBot;
Model *mBladeLeft;
Model *mBladeTop;
Model *mBladeRight;
Model *mFloor;
Model *mCastleWalls;
GLuint texWalls;
GLuint texRoof;
GLuint texBalcony;
GLuint texBlade;

mat4 mwWalls, mwRoof, mwBalcony, mwBladeBot, mwBladeLeft, mwBladeTop
	, mwBladeRight, mwFloor;
mat4 worldToProj;
mat4 projectionMatrix;
mat4 wv;
mat4 bladeRot;
vec3 camPlacement;

void controlFromKeyboard(vec3 *camPlacement)
{
	vec3 cam = *camPlacement;
	float rot = -PI*0.01;
	float scaling = 0.05;

	if (glutKeyIsDown('w'))
	{
		mat4 rotMat = Rx(rot);
		*camPlacement = MultVec3(rotMat, cam);
	}
	if (glutKeyIsDown('s'))
	{
		mat4 rotMat = Rx(-rot);
		*camPlacement = MultVec3(rotMat, cam);
	}
	if (glutKeyIsDown('d'))
	{
		mat4 rotMat = Ry(-rot);
		*camPlacement = MultVec3(rotMat, cam);
	}
	if (glutKeyIsDown('a'))
	{
		mat4 rotMat = Ry(rot);
		*camPlacement = MultVec3(rotMat, cam);
	}
	if (glutKeyIsDown('k'))
	{
		mat4 scaleMat = S(1.0 + scaling, 1.0 + scaling, 1.0 + scaling);
		*camPlacement = MultVec3(scaleMat, cam);
	}
	if (glutKeyIsDown('o'))
	{
		mat4 scaleMat = S(1.0 - scaling, 1.0 - scaling, 1.0 - scaling);
		*camPlacement = MultVec3(scaleMat, cam);
	}
}

void init(void)
{
	mFloor = LoadModelPlus("sol.obj");
	mWalls = LoadModelPlus("windmill-walls.obj");
	mRoof = LoadModelPlus("windmill-roof.obj");
	mBalcony = LoadModelPlus("windmill-balcony.obj");
	mBladeBot = LoadModelPlus("blade.obj");
	mBladeLeft = mBladeBot;
	mBladeTop = mBladeBot;
	mBladeRight = mBladeBot;

	dumpInfo();

	// GL inits
	glClearColor(0.3,0.3,0.3,0);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	//glCullFace(GL_FRONT);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab3-3.vert", "lab3-3.frag");
	printError("init shader");

	// Texture
	LoadTGATextureSimple("dirt.tga", &texWalls);
	LoadTGATextureSimple("dirt.tga", &texRoof);
	LoadTGATextureSimple("grass.tga", &texBlade);

	float rotTot = -PI/2;
	float transTot = -9;
	//Transformations
	mat4 bladeRotFixed = Rz(-PI/2);
	mwWalls = Mult(Ry(rotTot), T(0,transTot,0));
	mwRoof = Mult(Ry(rotTot), T(0,transTot,0));
	mwBalcony = Mult(Ry(rotTot), T(0,transTot,0));
	mwBladeBot = Mult(Ry(rotTot), S(0.6, 0.6, 0.6));
	mwBladeLeft = Mult(bladeRotFixed, mwBladeBot);
	mwBladeTop = Mult(bladeRotFixed, mwBladeLeft);
	mwBladeRight = Mult(bladeRotFixed, mwBladeTop);
	mwFloor = Mult(T(0,-12,0), S(50,10,50));


	projectionMatrix = frustum(left, right, bottom, top, near, far);
	vec3 camTemp = {0,0,30};
	camPlacement = camTemp;

	printError("init arrays");
}

void display(void)
{
	printError("pre display");
	t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

	vec3 *camPlacementPtr = &camPlacement;
	vec3 pointToLookAt = {0,0,0};
	vec3 upVector = {0,1,0};
	controlFromKeyboard(camPlacementPtr);
	wv = lookAtv(camPlacement, pointToLookAt, upVector);

	mat4 worldToProj =Mult(projectionMatrix, wv);
	glUniformMatrix4fv(glGetUniformLocation(program, "worldToProj"), 1, GL_TRUE
		, worldToProj.m);

	bladeRot = IdentityMatrix();
	glUniformMatrix4fv(glGetUniformLocation(program, "bladeRot")
		, 1, GL_TRUE, bladeRot.m);

	// clear the screen
	//glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glUniformMatrix4fv(glGetUniformLocation(program, "mw")
		, 1, GL_TRUE, mwFloor.m);
	DrawModel(mFloor, program, "in_Position"
		, "in_Normal", "inTexCoord");

	glUniformMatrix4fv(glGetUniformLocation(program, "mw")
		, 1, GL_TRUE, mwWalls.m);
	glBindTexture(GL_TEXTURE_2D, texWalls);
	glUniform1i(glGetUniformLocation(program, "in_texUnit"), 0);
	DrawModel(mWalls, program, "in_Position"
			, "in_Normal", "inTexCoord");

	glUniformMatrix4fv(glGetUniformLocation(program, "mw")
		, 1, GL_TRUE, mwRoof.m);
	glBindTexture(GL_TEXTURE_2D, texRoof);
	glUniform1i(glGetUniformLocation(program, "in_texUnit"), 0);
	DrawModel(mRoof, program, "in_Position"
		, "in_Normal", "inTexCoord");

	glUniformMatrix4fv(glGetUniformLocation(program, "mw")
		, 1, GL_TRUE, mwBalcony.m);
	glBindTexture(GL_TEXTURE_2D, texBalcony);
	glUniform1i(glGetUniformLocation(program, "in_texUnit"), 0);
	DrawModel(mBalcony, program, "in_Position"
		, "in_Normal", "inTexCoord");


	//BLADES

	glBindTexture(GL_TEXTURE_2D, texBlade);
	glUniform1i(glGetUniformLocation(program, "in_texUnit"), 0);

	bladeRot = Mult(Rz(0.001*t), T(0,0,5));
	glUniformMatrix4fv(glGetUniformLocation(program, "bladeRot")
		, 1, GL_TRUE, bladeRot.m);

	glUniformMatrix4fv(glGetUniformLocation(program, "mw")
		, 1, GL_TRUE, mwBladeBot.m);
	DrawModel(mBladeBot, program, "in_Position"
		, "in_Normal", "inTexCoord");

	glUniformMatrix4fv(glGetUniformLocation(program, "mw")
		, 1, GL_TRUE, mwBladeLeft.m);
	DrawModel(mBladeLeft, program, "in_Position"
		, "in_Normal", "inTexCoord");

	glUniformMatrix4fv(glGetUniformLocation(program, "mw")
		, 1, GL_TRUE, mwBladeTop.m);
	DrawModel(mBladeTop, program, "in_Position"
		, "in_Normal", "inTexCoord");

	glUniformMatrix4fv(glGetUniformLocation(program, "mw")
		, 1, GL_TRUE, mwBladeRight.m);
	DrawModel(mBladeRight, program, "in_Position"
		, "in_Normal", "inTexCoord");

	printError("display");
	glutSwapBuffers();
}

void OnTimer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(20, &OnTimer, value);
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutCreateWindow ("GL3 white triangle example");
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutDisplayFunc(display);
	init ();
	OnTimer(0);
	glutMainLoop();
	return 0;
}
