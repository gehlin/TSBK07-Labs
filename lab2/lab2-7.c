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
#include <stdio.h>
#include "loadobj.h"
#include "VectorUtils3.h"

#define PI 3.14
#define near 1.0
#define far 30.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5

/*GLfloat projectionMatrix[] = {2.0f*near/(right-left), 0.0f
	, (right+left)/(right-left), 0.0f, 0.0f, 2.0f*near/(top-bottom)
	, (top+bottom)/(top-bottom), 0.0f, 0.0f, 0.0f, -(far + near)/(far - near)
	, -2*far*near/(far - near), 0.0f, 0.0f, -1.0f, 0.0f };*/

GLfloat t;
GLuint program;
Model *mBunny;
Model *mCar;
GLuint texBunny;
GLuint texCar;

// vertex array object
unsigned int bunnyVertexArrayObjID;

mat4 modelToWorldBunny;
mat4 modelToWorldCar;
mat4 cameraRotation;
mat4 worldToProj;
mat4 projectionMatrix;
mat4 wv;

void init(void)
{
	mBunny = LoadModelPlus("bunnyplus.obj");
	mCar = LoadModelPlus("bilskiss.obj");

	dumpInfo();

	// GL inits
	glClearColor(0.0,0.0,0.0,0);

	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	//glCullFace(GL_FRONT);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab2-7.vert", "lab2-7.frag");
	printError("init shader");

	// Texture
	LoadTGATextureSimple("conc.tga", &texBunny);
	LoadTGATextureSimple("bilskissred.tga", &texCar);


	//Transformations
	modelToWorldBunny = T(1,0,0);
	modelToWorldCar = Mult(T(-1.6,0,0),Ry(PI*0.9));

	projectionMatrix = frustum(left, right, bottom, top, near, far);
	vec3 camPlacement = {0,0,8};
	vec3 pointToLookAt = {0,0,0};
	vec3 upVector = {0,1,0};
	wv = lookAtv(camPlacement, pointToLookAt, upVector);


	printError("init arrays");
}


void display(void)
{
	printError("pre display");

	// Animation
	mat4 rotAnim = Ry(0.0005*t);
	glUniformMatrix4fv(glGetUniformLocation(program, "rotAnim"), 1, GL_TRUE
		, rotAnim.m);

	cameraRotation = Ry(0.001*t);
	mat4 worldToProj =Mult(projectionMatrix, Mult(wv, cameraRotation));
	glUniformMatrix4fv(glGetUniformLocation(program, "worldToProj"), 1, GL_TRUE
		, worldToProj.m);

	// clear the screen
	//glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(bunnyVertexArrayObjID);


	glBindTexture(GL_TEXTURE_2D, texBunny);
	glUniform1i(glGetUniformLocation(program, "in_texUnit"), 0);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelToWorld")
		, 1, GL_TRUE, modelToWorldBunny.m);
	DrawModel(mBunny, program, "in_Position"
			, "in_Normal", "inTexCoord");

	glBindTexture(GL_TEXTURE_2D, texCar);
	glUniform1i(glGetUniformLocation(program, "in_texUnit"), 0);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelToWorld")
		, 1, GL_TRUE, modelToWorldCar.m);
	DrawModel(mCar, program, "in_Position"
		, "in_Normal", "inTexCoord");



	printError("display");

	glutSwapBuffers();
	t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	//glUniform1f(glGetUniformLocation(program, "time"),t);

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
