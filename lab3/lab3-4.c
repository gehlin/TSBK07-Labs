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

vec3 lightSourcesColorsArr[] = { {1.0f, 0.0f, 0.0f}, // Red light

                                 {0.0f, 1.0f, 0.0f}, // Green light

                                 {0.0f, 0.0f, 1.0f}, // Blue light

                                 {1.0f, 1.0f, 1.0f} }; // White light

GLint isDirectional[] = {0,0,1,1};

vec3 lightSourcesDirectionsPositions[] = { {10.0f, 5.0f, 0.0f}, // Red light, positional

                                       {0.0f, 5.0f, 10.0f}, // Green light, positional

                                       {-1.0f, 0.0f, 0.0f}, // Blue light along X

                                       {0.0f, 0.0f, -1.0f} }; // White light along Z

GLfloat specularExponent[] = {100.0, 200.0, 60.0, 50.0, 300.0, 150.0, 60.0, 50.0, 300.0, 150.0};



GLfloat t;
GLuint program;
GLuint programSkybox;

Model *mWalls;
Model *mRoof;
Model *mBalcony;
Model *mBladeBot;
Model *mBladeLeft;
Model *mBladeTop;
Model *mBladeRight;
Model *mFloor;
Model *mCastleWalls;
Model *mCastleRoof;
Model *mSkybox;

GLuint texWalls;
GLuint texRoof;
GLuint texBalcony;
GLuint texBlade;
GLuint texCastleWalls;
GLuint texCastleRoof;
GLuint texSkybox;
GLuint texFloor;

mat4 mwWalls, mwRoof, mwBalcony, mwBladeBot, mwBladeLeft, mwBladeTop
	, mwBladeRight, mwFloor, mwCastleWalls, mwCastleRoof, mwSkybox;
mat4 worldToProj;
mat4 projectionMatrix;
mat4 wv;
mat4 bladeRot;
vec3 camPlacement;
vec3 pointToLookAt;
vec3 upVector;

void controlFromKeyboard(vec3* camPlacement, vec3* pointToLookAt, vec3* upVector)
{
	vec3 cam = *camPlacement;
	vec3 lookAt = *pointToLookAt;
	vec3 up = *upVector;
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
	if (glutKeyIsDown('j'))
	{
		mat4 trans = T(cam.x, 0, cam.z);
		mat4 rotMat = Ry(rot);
		mat4 totRotMat = Mult(trans, Mult(rotMat, InvertMat4(trans)));
		*pointToLookAt = MultVec3(totRotMat, lookAt);
	}
	if (glutKeyIsDown('l'))
	{
		mat4 trans = T(cam.x, 0, cam.z);
		mat4 rotMat = Ry(-rot);
		mat4 totRotMat = Mult(trans, Mult(rotMat, InvertMat4(trans)));
		*pointToLookAt = MultVec3(totRotMat, lookAt);
	}
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
	mCastleWalls = LoadModelPlus("models/castle/walls.obj");
	mCastleRoof = LoadModelPlus("models/castle/rooftops.obj");
	mSkybox = LoadModelPlus("skybox.obj");

	dumpInfo();

	// GL inits
	glClearColor(0.3,0.3,0.3,0);

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	//glCullFace(GL_FRONT);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab3-4.vert", "lab3-4.frag");
	programSkybox = loadShaders("lab3-4skyBox.vert", "lab3-4skyBox.frag");
	printError("init shader");

	// Texture
	LoadTGATextureSimple("dirt.tga", &texWalls);
	LoadTGATextureSimple("dirt.tga", &texRoof);
	LoadTGATextureSimple("grass.tga", &texBlade);
	LoadTGATextureSimple("dirt.tga", &texCastleWalls);
	LoadTGATextureSimple("grass.tga", &texCastleRoof);
	LoadTGATextureSimple("SkyBox512.tga", &texSkybox);
	LoadTGATextureSimple("grass.tga", &texFloor);

	float rotTot = 3*PI/2;
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
	mat4 castleMat = Mult(T(-30, -10, 0), S(0.1,0.1,0.1));
	mwCastleWalls = castleMat;
	mwCastleRoof = castleMat;


	projectionMatrix = frustum(left, right, bottom, top, near, far);
	vec3 camTemp = {0,1,30};
	camPlacement = camTemp;
	vec3 pointTemp = {0,1,0};
	pointToLookAt = pointTemp;
	vec3 upTemp = {0,1,0};
	upVector = upTemp;

	printError("init arrays");
}

void display(void)
{
	printError("pre display");
	t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

	vec3* camPlacementPtr = &camPlacement;
	vec3* pointToLookAtPtr = &pointToLookAt;
	vec3* upVectorPtr = &upVector;
	controlFromKeyboard(camPlacementPtr, pointToLookAtPtr, upVectorPtr);
	wv = lookAtv(camPlacement, pointToLookAt, upVector);
	mwSkybox = Mult(T(camPlacement.x, camPlacement.y-0.2, camPlacement.z), S(1,1,1));
	mat4 worldToProj =Mult(projectionMatrix, wv);

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glUseProgram(programSkybox);
	glUniformMatrix4fv(glGetUniformLocation(programSkybox, "worldToProj"), 1, GL_TRUE
		, worldToProj.m);
	glUniformMatrix4fv(glGetUniformLocation(programSkybox, "mw")
		, 1, GL_TRUE, mwSkybox.m);
	glBindTexture(GL_TEXTURE_2D, texSkybox);
	glUniform1i(glGetUniformLocation(programSkybox, "in_texUnit"), 0);
	DrawModel(mSkybox, programSkybox, "in_Position"
			, NULL, "inTexCoord");


	glUseProgram(program);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	glUniform3fv(glGetUniformLocation(program, "lightSourcesDirPosArr"), 4, &lightSourcesDirectionsPositions);

	glUniform3fv(glGetUniformLocation(program, "lightSourcesColorArr"), 4, &lightSourcesColorsArr);

	glUniform1f(glGetUniformLocation(program, "specularExponent"), specularExponent[0]);

	glUniform1iv(glGetUniformLocation(program, "isDirectional"), 4, isDirectional);

	glUniform3fv(glGetUniformLocation(program, "camPlacement"), 1, &camPlacement);


	glUniformMatrix4fv(glGetUniformLocation(program, "worldToProj"), 1, GL_TRUE
		, worldToProj.m);

	bladeRot = IdentityMatrix();
	glUniformMatrix4fv(glGetUniformLocation(program, "bladeRot")
		, 1, GL_TRUE, bladeRot.m);

	glUniformMatrix4fv(glGetUniformLocation(program, "mw")
		, 1, GL_TRUE, mwFloor.m);
	glBindTexture(GL_TEXTURE_2D, texFloor);
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


	glUniformMatrix4fv(glGetUniformLocation(program, "mw")
		, 1, GL_TRUE, mwCastleWalls.m);
	glBindTexture(GL_TEXTURE_2D, texCastleWalls);
	DrawModel(mCastleWalls, program, "in_Position"
		, "in_Normal", "inTexCoord");

	glUniformMatrix4fv(glGetUniformLocation(program, "mw")
		, 1, GL_TRUE, mwCastleRoof.m);
	glBindTexture(GL_TEXTURE_2D, texCastleRoof);
	DrawModel(mCastleRoof, program, "in_Position"
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
