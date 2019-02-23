// Lab 4, terrain generation

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"
#include "stdio.h"

#define PI 3.14

mat4 projectionMatrix;

void controlFromKeyboard(vec3* camPlacement, vec3* pointToLookAt)
{
	vec3 cam = *camPlacement;
	vec3 lookAt = *pointToLookAt;
	vec3 up = {0,1,0};
	vec3 forwardDirection = VectorSub(lookAt, cam);
	vec3 rightDirection = CrossProduct(forwardDirection, up);
	float rot = PI*0.02;
	float scaling = 0.05;

	if (glutKeyIsDown('w'))
	{
		mat4 trans = T(scaling*forwardDirection.x, scaling*forwardDirection.y, scaling*forwardDirection.z);
		*camPlacement = MultVec3(trans, cam);
		*pointToLookAt = MultVec3(trans, lookAt);
	}
	if (glutKeyIsDown('s'))
	{
		mat4 trans = T(-scaling*forwardDirection.x, -scaling*forwardDirection.y, -scaling*forwardDirection.z);
		*camPlacement = MultVec3(trans, cam);
		*pointToLookAt = MultVec3(trans, lookAt);
	}
	if (glutKeyIsDown('d'))
	{
		mat4 trans = T(scaling*rightDirection.x, scaling*rightDirection.y, scaling*rightDirection.z);
		*camPlacement = MultVec3(trans, cam);
		*pointToLookAt = MultVec3(trans, lookAt);
	}
	if (glutKeyIsDown('a'))
	{
		mat4 trans = T(-scaling*rightDirection.x, -scaling*rightDirection.y, -scaling*rightDirection.z);
		*camPlacement = MultVec3(trans, cam);
		*pointToLookAt = MultVec3(trans, lookAt);
	}
}


Model* GenerateTerrain(TextureData *tex)
{
	int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width-1) * (tex->height-1) * 2;
	//printf(triangleCount);
	printf("%d\n", triangleCount);
	int x, z;

	GLfloat *vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount*3);

	printf("bpp %d\n", tex->bpp);
	//printf(tex->imageData);
	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
// Vertex array. You need to scale this properly
			vertexArray[(x + z * tex->width)*3 + 0] = x / 20.0;
			vertexArray[(x + z * tex->width)*3 + 1] = tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / 70.0;
			vertexArray[(x + z * tex->width)*3 + 2] = z / 20.0;
// Normal vectors. You need to calculate these.
			normalArray[(x + z * tex->width)*3 + 0] = 0.0;
			normalArray[(x + z * tex->width)*3 + 1] = 1.0;
			normalArray[(x + z * tex->width)*3 + 2] = 0.0;
// Texture coordinates. You may want to scale them.
			texCoordArray[(x + z * tex->width)*2 + 0] = x; // (float)x / tex->width;
			texCoordArray[(x + z * tex->width)*2 + 1] = z; // (float)z / tex->height;
		}
	for (x = 0; x < tex->width-1; x++)
		for (z = 0; z < tex->height-1; z++)
		{
		// Triangle 1
			indexArray[(x + z * (tex->width-1))*6 + 0] = x + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 1] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 2] = x+1 + z * tex->width;
		// Triangle 2
			indexArray[(x + z * (tex->width-1))*6 + 3] = x+1 + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 4] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 5] = x+1 + (z+1) * tex->width;
		}

	// End of terrain generation

	// Create Model and upload to GPU:

	Model* model = LoadDataToModel(
			vertexArray,
			normalArray,
			texCoordArray,
			NULL,
			indexArray,
			vertexCount,
			triangleCount*3);

	return model;
}


// vertex array object
Model *m, *m2, *tm;
// Reference to shader program
GLuint program;
GLuint tex1, tex2;
TextureData ttex; // terrain
vec3 cam, lookAtPoint, upVector;

void init(void)
{
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 50.0);

	// Load and compile shader
	program = loadShaders("terrain.vert", "terrain.frag");
	glUseProgram(program);
	printError("init shader");

	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
	LoadTGATextureSimple("maskros512.tga", &tex1);

// Load terrain data

	LoadTGATextureData("fft-terrain.tga", &ttex);
	tm = GenerateTerrain(&ttex);
	printError("init terrain");

	vec3 camTemp = {0, 5, 8};
	cam = camTemp;
	vec3 lookTemp = {2, 0, 2};
	lookAtPoint = lookTemp;
	vec3 upTemp = {0,1,0};
	upVector = upTemp;
}

void display(void)
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 total, modelView, camMatrix;

	printError("pre display");

	glUseProgram(program);

	// Build matrix

	controlFromKeyboard(&cam, &lookAtPoint);
	camMatrix = lookAt(cam.x, cam.y, cam.z,
				lookAtPoint.x, lookAtPoint.y, lookAtPoint.z,
				upVector.x, upVector.y, upVector.z);
	modelView = IdentityMatrix();
	total = Mult(camMatrix, modelView);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);

	glBindTexture(GL_TEXTURE_2D, tex1);		// Bind Our Texture tex1
	DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");

	printError("display 2");

	glutSwapBuffers();
}

void timer(int i)
{
	glutTimerFunc(20, &timer, i);
	glutPostRedisplay();
}

int sign(int x) {
    return (x > 0) - (x < 0);
}

int oldX = 0;
int oldY = 0;
// void (*func)(int button, int state, int x, int y)
//void mouse(int button, int state, int x, int y)
void mouse(int x, int y)
{

		int deltaX = x-oldX;
		int deltaY = y-oldY;
		float rot = PI*0.008;
		vec3 forwardDirection = VectorSub(lookAtPoint, cam);
		vec3 rightDirection = CrossProduct(forwardDirection, upVector);

		// Rotate up-down
		vec3 upRotAxis = {rightDirection.x, 0, rightDirection.z};
		mat4 upRotMat = ArbRotate(upRotAxis, sign(deltaY)*rot);

		// Rotate left-right
		mat4 trans = T(cam.x, 0, cam.z);
		mat4 rotMat = Ry(sign(deltaX)*rot);

		// Total rotation
		mat4 totRotMat = Mult(trans, Mult(Mult(rotMat, upRotMat), InvertMat4(trans)));
		lookAtPoint = MultVec3(totRotMat, lookAtPoint);
		//upVector = MultVec3(upRotMat, upVector);





	/*if (glutKeyIsDown('k'))
	{
		mat4 rotMat = ArbRotate(rightDirection, rot);
		*pointToLookAt = MultVec3(rotMat, lookAt);
		vec3 upRotAxis = {rightDirection.x, 0, rightDirection.z};
		mat4 upRotMat = ArbRotate(upRotAxis, rot);
		*upVector = MultVec3(upRotMat, up);
	}
	if (glutKeyIsDown('i'))
	{
		mat4 rotMat = ArbRotate(rightDirection, -rot);
		*pointToLookAt = MultVec3(rotMat, lookAt);
		vec3 upRotAxis = {rightDirection.x, 0, rightDirection.z};
		mat4 upRotMat = ArbRotate(upRotAxis, -rot);
		*upVector = MultVec3(upRotMat, up);
	}*/
	//printf("%d %d\n", x, y);
	oldX = x;
	oldY = y;
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize (600, 600);
	glutCreateWindow ("TSBK07 Lab 4");
	glutDisplayFunc(display);
	init ();
	glutTimerFunc(20, &timer, 0);

	glutMotionFunc(mouse);
	//glutPassiveMotionFunc(mouse);
	//glutMouseFunc(void (*func)(int button, int state, int x, int y))
	//glutMouseFunc(mouse);

	glutMainLoop();
	exit(0);
}
