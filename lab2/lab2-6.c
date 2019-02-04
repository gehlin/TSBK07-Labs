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
GLuint tex;

// vertex array object
unsigned int bunnyVertexArrayObjID;

void init(void)
{
	mBunny = LoadModel("bunnyplus.obj");

	unsigned int bunnyVertexBufferObjID;
	unsigned int bunnyIndexBufferObjID;
	unsigned int bunnyNormalBufferObjID;
	unsigned int bunnyTexCoordBufferObjID;

	dumpInfo();

	// GL inits
	glClearColor(0.0,0.0,0,0);

	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	//glCullFace(GL_FRONT);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab2-5.vert", "lab2-5.frag");
	printError("init shader");

	//
	glGenVertexArrays(1, &bunnyVertexArrayObjID);
    glGenBuffers(1, &bunnyVertexBufferObjID);
    glGenBuffers(1, &bunnyIndexBufferObjID);
    glGenBuffers(1, &bunnyNormalBufferObjID);
	glBindVertexArray(bunnyVertexArrayObjID);

	glGenBuffers(1, &bunnyTexCoordBufferObjID);
   	if (mBunny->texCoordArray != NULL)
   	{
	   	glBindBuffer(GL_ARRAY_BUFFER, bunnyTexCoordBufferObjID);
	   	glBufferData(GL_ARRAY_BUFFER, mBunny->numVertices*2*sizeof(GLfloat)
			, mBunny->texCoordArray, GL_STATIC_DRAW);
	   	glVertexAttribPointer(glGetAttribLocation(program, "inTexCoord"), 2
			, GL_FLOAT, GL_FALSE, 0, 0);
	   	glEnableVertexAttribArray(glGetAttribLocation(program, "inTexCoord"));
	}

	// VBO for vertex data
    glBindBuffer(GL_ARRAY_BUFFER, bunnyVertexBufferObjID);
    glBufferData(GL_ARRAY_BUFFER, mBunny->numVertices*3*sizeof(GLfloat)
		, mBunny->vertexArray, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3
		, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));

	// VBO for normal data
    glBindBuffer(GL_ARRAY_BUFFER, bunnyNormalBufferObjID);
    glBufferData(GL_ARRAY_BUFFER, mBunny->numVertices*3*sizeof(GLfloat)
		, mBunny->normalArray, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(program, "in_Normal"), 3
		, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(program, "in_Normal"));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bunnyIndexBufferObjID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mBunny->numIndices*sizeof(GLuint)
		, mBunny->indexArray, GL_STATIC_DRAW);

	// End of upload of geometry

	// Texture
	LoadTGATextureSimple("conc.tga", &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(glGetUniformLocation(program, "in_texUnit"), 0);

	//Transformations
	mat4 projectionMatrix = frustum(left, right, bottom, top, near, far);
	vec3 camPlacement = {-1,0,2};
	vec3 pointToLookAt = {0,0,0};
	vec3 upVector = {0,1,0};
	mat4 wv = lookAtv(camPlacement, pointToLookAt, upVector);
	mat4 totMat =Mult(projectionMatrix, wv);
	glUniformMatrix4fv(glGetUniformLocation(program, "totMat"), 1, GL_TRUE
		, totMat.m);

	printError("init arrays");
}


void display(void)
{
	printError("pre display");

	// Animation
	mat4 rotAnim = Ry(0.001*t);
	glUniformMatrix4fv(glGetUniformLocation(program, "rotAnim"), 1, GL_TRUE
		, rotAnim.m);

	// clear the screen
	//glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(bunnyVertexArrayObjID);    // Select VAO
    glDrawElements(GL_TRIANGLES, mBunny->numIndices, GL_UNSIGNED_INT, 0L);

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
