#pragma once
#include "DrawModelShader.h"
#include "DrawPickingFaceShader.h"
#include "DrawPointShader.h"
#include "PickingShader.h"
#include "PickingTexture.h"
#include "MeshObject.h"
#include "ViewManager.h"


#define LEFT_BUTTON   0x0000
#define MIDDLE_BUTTON 0x0001
#define RIGHT_BUTTON  0x0002
#define WHEEL_UP     1
#define WHEEL_DOWN  -1

#define ADD_FACE		1
#define SELECT_POINT	2
#define DEL_FACE		3
extern int PickMode = 0;

using namespace std;

GLuint			program;			// shader program
glm::mat4		proj_matrix;		// projection matrix
MeshObject		model;
ViewManager		camera1;
GLuint vboPoint;					// vbo for drawing point

glm::vec3 worldPos;
bool updateFlag = false;
bool isRightButtonPress = false;
GLuint currentFaceID = 0;
int currentMouseX = 0;
int currentMouseY = 0;

float deltaTime = 0.015;
float aspect;
float windowWidth = 509.0;
float windowHeight = 454.0;

// ****** SHADER ******
DrawModelShader drawModelShader;
DrawPickingFaceShader drawPickingFaceShader;
PickingShader pickingShader;
DrawPointShader drawPointShader;
PickingTexture pickingTexture;

void My_LoadModel()
{
	if (model.Init(ResourcePath::modelPath))
	{
		puts("Load Model Success");
	}
	else
	{
		puts("Load Model Failed");
	}
}

void InitOpenGL()
{
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_POINT_SMOOTH);
}


void InitData()
{
	ResourcePath::shaderPath = "./Shader/";
	ResourcePath::modelPath = "./resource/3DModel/UnionSphere.obj";

	//Initialize shaders
	if (drawModelShader.Init())
	{
		puts("drawModelShader Success");
	}
	else
	{
		puts("drawModelShader Failed");
	}

	if (pickingShader.Init())
	{
		puts("pickingShader Success");
	}
	else
	{
		puts("pickingShader Failed");
	}
	
	if (drawPickingFaceShader.Init())
	{
		puts("drawPickingFaceShader Success");
	}
	else
	{
		puts("drawPickingFaceShader Failed");
	}
	
	if (drawPointShader.Init())
	{
		puts("drawPointShader Success");
	}
	else
	{
		puts("drawPointShader Failed");
	}
	if (pickingTexture.Init(windowWidth, windowHeight))
	{
		puts("pickingTexture Success");
	}
	else
	{
		puts("pickingTexture Failed");
	}
	
	/*drawModelShader.Init();
	pickingShader.Init();
	drawPickingFaceShader.Init();
	drawPointShader.Init();*/

	glGenBuffers(1, &vboPoint);

	//Load model to shader program
	My_LoadModel();
	cout << "end of InitData" << endl;

	aspect = windowWidth * 1.0f / windowHeight;
}

void Reshape(int width, int height)
{
	windowWidth = width;
	windowHeight = height;


	glutReshapeWindow(windowWidth, windowHeight);
	glViewport(0, 0, windowWidth, windowHeight);

	aspect = windowWidth * 1.0f / windowHeight;
	camera1.SetWindowSize(windowWidth, windowHeight);
	pickingTexture.Init(windowWidth, windowHeight);
}



void RenderMeshWindow()
{
	//Update shaders' input variable	 
	glm::mat4 mvMat = camera1.GetViewMatrix() * camera1.GetModelMatrix();
	glm::mat4 pMat = camera1.GetProjectionMatrix(aspect);

	// write faceID+1 to framebuffer
	pickingTexture.Enable();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	pickingShader.Enable();
	pickingShader.SetMVMat(value_ptr(mvMat));
	pickingShader.SetPMat(value_ptr(pMat));

	model.Render();

	pickingShader.Disable();
	pickingTexture.Disable();

	// draw model
	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawModelShader.Enable();
	glm::mat3 normalMat = glm::transpose(glm::inverse(glm::mat3(mvMat)));

	drawModelShader.SetWireColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	drawModelShader.SetFaceColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	drawModelShader.UseLighting(true);
	drawModelShader.DrawWireframe(true);
	drawModelShader.SetNormalMat(normalMat);
	drawModelShader.SetMVMat(mvMat);
	drawModelShader.SetPMat(pMat);

	model.Render();

	drawModelShader.Disable();

	// render selected face
	if ( PickMode == ADD_FACE || PickMode == DEL_FACE)
	{
		cout << PickMode << " in render selected face" << endl;
		drawPickingFaceShader.Enable();
		drawPickingFaceShader.SetMVMat(value_ptr(mvMat));
		drawPickingFaceShader.SetPMat(value_ptr(pMat));
		model.RenderSelectedFace();
		drawPickingFaceShader.Disable();
	}

	glUseProgram(0);

	// render closest point
	if (PickMode == SELECT_POINT)
	{
		cout << PickMode << " in render closest point" << endl;
		if (updateFlag)
		{
			float depthValue = 0;
			int windowX = currentMouseX;
			int windowY = windowHeight - currentMouseY;
			glReadPixels(windowX, windowY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depthValue);

			GLint _viewport[4];
			glGetIntegerv(GL_VIEWPORT, _viewport);
			glm::vec4 viewport(_viewport[0], _viewport[1], _viewport[2], _viewport[3]);
			glm::vec3 windowPos(windowX, windowY, depthValue);
			glm::vec3 wp = glm::unProject(windowPos, mvMat, pMat, viewport);
			model.FindClosestPoint(currentFaceID - 1, wp, worldPos);

			updateFlag = false;
		}


		glBindBuffer(GL_ARRAY_BUFFER, vboPoint);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3), glm::value_ptr(worldPos), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glm::vec4 pointColor(1.0, 1.0, 0.0, 1.0);
		drawPointShader.Enable();
		drawPointShader.SetMVMat(mvMat);
		drawPointShader.SetPMat(pMat);
		drawPointShader.SetPointColor(pointColor);
		drawPointShader.SetPointSize(15.0);

		glDrawArrays(GL_POINTS, 0, 1);

		drawPointShader.Disable();

		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}
}

void SelectionHandler(unsigned int x, unsigned int y)
{
	GLuint faceID = pickingTexture.ReadTexture(x, windowHeight - y - 1);
	if (faceID != 0)
	{
		currentFaceID = faceID;
	}

	if (PickMode == ADD_FACE)
	{
		if (faceID != 0)
		{
			model.AddSelectedFace(faceID - 1);
		}
	}
	else if (PickMode == DEL_FACE)
	{
		if (faceID != 0)
		{
			model.DeleteSelectedFace(faceID - 1);
		}
	}
	else if (PickMode == SELECT_POINT)
	{
		currentMouseX = x;
		currentMouseY = y;
		updateFlag = true;
	}
}

void Mouse_Press(int button, int x, int y)
{
	camera1.mousePressEvent(button, x, y);
	
	// Right Button Actived
	if (button == RIGHT_BUTTON)
	{
		isRightButtonPress = true;
		SelectionHandler(x, y);
	}
}

void Mouse_Release(int button, int x, int y)
{
	camera1.mouseReleaseEvent(button, x, y);

	// Right Button unActived
	if (button == RIGHT_BUTTON)
	{
		isRightButtonPress = false;
	}
}

void Mouse_Moving(int x, int y)
{
	camera1.mouseMoveEvent(x, y);

	// Right Button Actived ==> Selecting
	if (isRightButtonPress)
	{
		SelectionHandler(x, y);
	}
}

void Mouse_Wheel(int state)
{
	camera1.wheelEvent(state);
}

void keypress_function(Camera_Movement stat)
{
	camera1.ProcessKeyboard(stat, deltaTime);
}