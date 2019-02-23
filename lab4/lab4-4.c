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
	//float rot = PI*0.02;
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

vec3 getNormalVector(vec3 v1, vec3 v2, vec3 v3)
{
	vec3 u = VectorSub(v1,v2);
	vec3 v = VectorSub(v3,v1);
	return  Normalize(CrossProduct(u,v));
}

float getHeightInt(int x, int z, GLfloat *vertexArray, TextureData *tex)
{
	return vertexArray[(x + z * tex->width)*3 + 1];
}

float getHeight(float x, float z, GLfloat *vertexArray, TextureData *tex)
{
	x = x * 20;
	z = z * 20;
	printf("%f\n", x);
	float p1 = getHeightInt(floor(x), ceil(z), vertexArray, tex);
	float p2 = getHeightInt(ceil(x), ceil(z), vertexArray, tex);
	float p3 = getHeightInt(floor(x), floor(z), vertexArray, tex);
	float p4 = getHeightInt(ceil(x), floor(z), vertexArray, tex);
	float x2 = x-floor(x);
	float x1 = 1-x2;
	float A = x1*p1 + x2*p2;
	float B = x1*p3 + x2*p4;
	float zA = z-floor(z);
	float zB = 1-zA;
	float height = zA*A + zB*B;
	return height;
	//return getHeightInt(round(x), round(z), vertexArray, tex);
}

Model* GenerateTerrain(TextureData *tex)
{
	int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width-1) * (tex->height-1) * 2;
	//printf(triangleCount);
	//printf("%d\n", triangleCount);
	int x, z;

	GLfloat *vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount*3);

	//printf("bpp %d\n", tex->bpp);
	//printf(tex->imageData);
	int count = 0;
	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
// Vertex array. You need to scale this properly
			//printf("tex->width: %d\n", tex->width);
			vertexArray[(x + z * tex->width)*3 + 0] = x / 20.0;
			vertexArray[(x + z * tex->width)*3 + 1] = tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / 70.0;
			vertexArray[(x + z * tex->width)*3 + 2] = z / 20.0;
// Normal vectors. You need to calculate these.
		//	printf(x);
		//	printf(z);
			if (z>1 && x>1)
			{
				vec3 vertex1 = {vertexArray[((x-1-1) + (z-1-1) * tex->width)*3 + 0]
					, vertexArray[((x-1-1) + (z-1-1) * tex->width)*3 + 1]
					, vertexArray[((x-1-1) + (z-1-1) * tex->width)*3 + 2]};
				vec3 vertex2 = {vertexArray[(x + (z-1-1) * tex->width)*3 + 0]
					, vertexArray[(x + (z-1-1) * tex->width)*3 + 1]
					, vertexArray[(x + (z-1-1) * tex->width)*3 + 2]};
				vec3 vertex3 = {vertexArray[(x + z * tex->width)*3 + 0]
					, vertexArray[(x + z * tex->width)*3 + 1]
					, vertexArray[(x + z * tex->width)*3 + 2]};

				vec3 normalVector = getNormalVector(vertex1, vertex2, vertex3);

				normalArray[((x-1) + (z-1) * tex->width)*3 + 0] = normalVector.x;
				normalArray[((x-1) + (z-1) * tex->width)*3 + 1] = normalVector.y;
				normalArray[((x-1) + (z-1) * tex->width)*3 + 2] = normalVector.z;
			}
			else
			{
				//printf("bajs");
				//printf((x + z * tex->width)*3 + 0);
				normalArray[(x + z * tex->width)*3 + 0] = 0;
				normalArray[(x + z * tex->width)*3 + 1] = 1;
				normalArray[(x + z * tex->width)*3 + 2] = 0;
			}

// Texture coordinates. You may want to scale them.
			texCoordArray[(x + z * tex->width)*2 + 0] = x; // (float)x / tex->width;
			texCoordArray[(x + z * tex->width)*2 + 1] = z; // (float)z / tex->height;
			count++;
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
Model *m, *m2, *tm, *mBall;
// Reference to shader program
GLuint program;
GLuint tex1, tex2, texBall;
TextureData ttex; // terrain
vec3 cam, lookAtPoint, upVector;
mat4 mwBall;

void init(void)
{
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 50.0);

	// Load and compile shader
	program = loadShaders("lab4-4.vert", "lab4-4.frag");
	glUseProgram(program);
	printError("init shader");

	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
	LoadTGATextureSimple("grass.tga", &tex1);

	LoadTGATextureSimple("dirt.tga", &texBall);


	mBall = LoadModelPlus("groundsphere.obj");
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

	float t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

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
	float t1 = 10*(sin(0.0001*t)+1)/2;
	mat4 trans = T(t1,getHeight(t1,t1,tm->vertexArray, &ttex),t1);
	//printf("%f\n", getHeight(t1,t1,tm->vertexArray, &ttex));
	float s = 0.1;
	mat4 scale = S(s,s,s);
	mwBall = Mult(total, Mult(trans, scale));
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, mwBall.m);
	glBindTexture(GL_TEXTURE_2D, texBall);		// Bind Our Texture tex1
	DrawModel(mBall, program, "inPosition", "inNormal", "inTexCoord");

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
