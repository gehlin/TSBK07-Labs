// Lab 1-1.
// This is the same as the first simple example in the course book,
// but with a few error checks.
// Remember to copy your file to a new on appropriate places during the lab so you keep old results.
// Note that the files "lab1-1.frag", "lab1-1.vert" are required.

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

// Globals
// Data would normally be read from files
GLfloat vertices[] =
{
	-0.5f,-0.5f,-0.5f, // triangle 1 : begin
    -0.5f,-0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f, // triangle 1 : end
    0.5f, 0.5f,-0.5f, // triangle 2 : begin
    -0.5f,-0.5f,-0.5f,
    -0.5f, 0.5f,-0.5f, // triangle 2 : end
    0.5f,-0.5f, 0.5f,
    -0.5f,-0.5f,-0.5f,
    0.5f,-0.5f,-0.5f,
    0.5f, 0.5f,-0.5f,
    0.5f,-0.5f,-0.5f,
    -0.5f,-0.5f,-0.5f,
    -0.5f,-0.5f,-0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f,-0.5f,
    0.5f,-0.5f, 0.5f,
    -0.5f,-0.5f, 0.5f,
    -0.5f,-0.5f,-0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f,-0.5f, 0.5f,
    0.5f,-0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f,-0.5f,-0.5f,
    0.5f, 0.5f,-0.5f,
    0.5f,-0.5f,-0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f,-0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f,-0.5f,
    -0.5f, 0.5f,-0.5f,
    0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f,-0.5f,
    -0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
    0.5f,-0.5f, 0.5f
};



GLfloat color[] =
{
	1.0f, 0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,//
    1.0f, 1.0f, 0.5f, 1.0f,
    0.5f, 1.0f, 0.5f, 1.0f,
    0.5f, 1.0f, 1.0f, 1.0f,
    1.0f, 0.5f, 1.0f, 1.0f,
    0.5f, 1.0f, 1.0f, 1.0f,
    0.5f, 0.5f, 1.0f, 1.0f,
    1.0f, 0.5f, 0.5f, 1.0f,
    1.0f, 1.0f, 0.5f, 1.0f,
    1.0f, 0.5f, 1.0f, 1.0f,
    1.0f, 0.5f, 1.0f, 1.0f,
    1.0f, 1.0f, 0.5f, 1.0f,
    0.5f, 1.0f, 1.0f, 1.0f,
    1.0f, 0.5f, 1.0f, 1.0f,
    0.5f, 1.0f, 0.5f, 1.0f,
    0.5f, 1.0f, 1.0f, 1.0f,
	0.4f, 0.5f, 0.5f, 1.0f,
    0.8f, 1.0f, 0.5f, 1.0f,
    0.5f, 0.5f, 1.0f, 1.0f,
	1.0f, 0.5f, 0.5f, 1.0f,
    0.1f, 1.0f, 0.3f, 1.0f,
    0.5f, 0.1f, 1.0f, 1.0f,
	1.0f, 0.8f, 0.1f, 1.0f,
    0.2f, 1.0f, 0.5f, 1.0f,
    0.5f, 0.5f, 1.0f, 1.0f,
	1.0f, 0.5f, 0.5f, 1.0f,
    0.5f, 0.2f, 0.5f, 1.0f,
    0.5f, 0.5f, 0.8f, 1.0f,
	1.0f, 0.5f, 0.5f, 1.0f,
    0.5f, 1.0f, 0.2f, 1.0f,
    0.5f, 1.0f, 0.1f, 1.0f,
	1.0f, 0.5f, 0.5f, 1.0f,
    0.5f, 1.0f, 0.5f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f
};


GLfloat myMatrix[] = {  1.0f, 0.0f, 0.0f, 0.0f,

						0.0f, 1.0f, 0.0f, 0.0f,

						0.0f, 0.0f, 1.0f, 0.0f,

						0.0f, 0.0f, 0.0f, 1.0f };

GLfloat t;
GLuint program;


// vertex array object
unsigned int vertexArrayObjID;

void init(void)
{
	// vertex buffer object, used for uploading the geometry
	unsigned int vertexBufferObjID;
	unsigned int colorBufferObjID;
	// Reference to shader program

	dumpInfo();

	// GL inits
	glClearColor(0.0,0.0,0,0);
	glEnable(GL_CULL_FACE);
	//glDisable(GL_DEPTH_TEST);
	//glCullFace(GL_FRONT);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab1-5.vert", "lab1-5.frag");
	printError("init shader");

	// Upload geometry to the GPU:

	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);
	// Allocate Vertex Buffer Objects
	glGenBuffers(1, &vertexBufferObjID);
	glGenBuffers(1, &colorBufferObjID);

	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 12*9*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));

	glBindBuffer(GL_ARRAY_BUFFER, colorBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 12*3*4*sizeof(GLfloat), color, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "color"), 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "color"));


	// End of upload of geometry

	printError("init arrays");
}


void display(void)
{
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(vertexArrayObjID);	// Select VAO
	glDrawArrays(GL_TRIANGLES, 0, 12*3);	// draw object

	printError("display");

	glutSwapBuffers();
	t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	myMatrix[0] = cos(0.001*t);
	myMatrix[1] = -1*sin(0.001*t);
	myMatrix[4] = sin(0.001*t);
	myMatrix[5] = cos(0.001*t);
	myMatrix[6] = -sin(0.001*t);
	myMatrix[9] = sin(0.001*t);
	//myMatrix[3] = sin(0.002*t);
	glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, myMatrix);
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
