#pragma once
#include "DrawModelShader.h"
#include "DrawPickingFaceShader.h"
#include "DrawPointShader.h"
#include "PickingShader.h"
#include "PickingTexture.h"
#include "MeshObject.h"
#include "ViewManager.h"


#define WHEEL_UP     1
#define WHEEL_DOWN  -1
using namespace std;

GLuint			program;			// shader program
glm::mat4		proj_matrix;		// projection matrix
MeshObject		model;
ViewManager		camera1;
GLuint vboPoint;					// vbo for drawing point

float deltaTime = 0.015;
float aspect;
float windowWidth = 613.0f;
float windowHeight = 430.0f;

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
	drawModelShader.Init();
	pickingShader.Init();
	drawPickingFaceShader.Init();
	drawPointShader.Init();

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
	/*if (selectionMode == SelectionMode::ADD_FACE || selectionMode == SelectionMode::DEL_FACE)
	{
		drawPickingFaceShader.Enable();
		drawPickingFaceShader.SetMVMat(value_ptr(mvMat));
		drawPickingFaceShader.SetPMat(value_ptr(pMat));
		model.RenderSelectedFace();
		drawPickingFaceShader.Disable();
	}*/

	glUseProgram(0);
}

void Mouse_Press(int button, int x, int y)
{
	camera1.mousePressEvent(button, x, y);
}

void Mouse_Release(int button, int x, int y)
{
	camera1.mouseReleaseEvent(button, x, y);
}

void Mouse_Moving(int x, int y)
{
	camera1.mouseMoveEvent(x, y);
}

void Mouse_Wheel(int state)
{
	camera1.wheelEvent(state);
}

void keypress_function(Camera_Movement stat)
{
	camera1.ProcessKeyboard(stat, deltaTime);
}