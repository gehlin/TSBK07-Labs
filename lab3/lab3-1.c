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
#define far 30.0
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
GLuint texWalls;
GLuint texRoof;
GLuint texBalcony;
GLuint texBlade;

mat4 mwWalls, mwRoof, mwBalcony, mwBladeBot, mwBladeLeft, mwBladeTop
	, mwBladeRight;
mat4 worldToProj;
mat4 projectionMatrix;
mat4 wv;
mat4 bladeRot;

void init(void)
{
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

	//glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_FRONT);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab3-1.vert", "lab3-1.frag");
	printError("init shader");

	// Texture
	LoadTGATextureSimple("dirt.tga", &texWalls);
	LoadTGATextureSimple("dirt.tga", &texRoof);

	float rotTot = -PI/2;
	float transTot = -9;
	//Transformations
	mat4 bladeRotFixed = Rz(-PI/2);
	mwWalls = Mult(Ry(rotTot), T(0,transTot,0));
	mwRoof = Mult(Ry(rotTot), T(0,transTot,0));
	mwBalcony = Mult(Ry(rotTot), T(0,transTot,0));
	mwBladeBot = Mult(T(0,0,0), Ry(rotTot));
	mwBladeLeft = Mult(bladeRotFixed, modelToWorldBladeBot);
	mwBladeTop = Mult(bladeRotFixed, modelToWorldBladeLeft);
	mwBladeRight = Mult(bladeRotFixed, modelToWorldBladeTop);


	projectionMatrix = frustum(left, right, bottom, top, near, far);
	vec3 camPlacement = {0,0,30};
	vec3 pointToLookAt = {0,0,0};
	vec3 upVector = {0,1,0};
	wv = lookAtv(camPlacement, pointToLookAt, upVector);


	printError("init arrays");
}

void display(void)
{
	printError("pre display");
	t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

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
		, 1, GL_TRUE, mwWalls.m);
	DrawModel(mWalls, program, "in_Position"
			, "in_Normal", "inTexCoord");

	glUniformMatrix4fv(glGetUniformLocation(program, "mw")
		, 1, GL_TRUE, mwRoof.m);
	DrawModel(mRoof, program, "in_Position"
		, "in_Normal", "inTexCoord");

	glUniformMatrix4fv(glGetUniformLocation(program, "mw")
		, 1, GL_TRUE, mwBalcony.m);
	DrawModel(mBalcony, program, "in_Position"
		, "in_Normal", "inTexCoord");


	//BLADES

	bladeRot = Mult(Rz(0.001*t), T(0,0,5));
	glUniformMatrix4fv(glGetUniformLocation(program, "bladeRot")
		, 1, GL_TRUE, bladeRot.m);

	glUniformMatrix4fv(glGetUniformLocation(program, "mw")
		, 1, GL_TRUE, mwBladeBot.m);
	DrawModel(mBladeBot, program, "in_Position"
		, "in_Normal", "inTexCoord");

	glUniformMatrix4fv(glGetUniformLocation(program, "modelToWorld")
		, 1, GL_TRUE, mwBladeLeft.m);
	DrawModel(mBladeLeft, program, "in_Position"
		, "in_Normal", "inTexCoord");

	glUniformMatrix4fv(glGetUniformLocation(program, "modelToWorld")
		, 1, GL_TRUE, mwBladeTop.m);
	DrawModel(mBladeTop, program, "in_Position"
		, "in_Normal", "inTexCoord");

	glUniformMatrix4fv(glGetUniformLocation(program, "modelToWorld")
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
