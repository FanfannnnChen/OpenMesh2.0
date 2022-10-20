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

//#define ADD_FACE		1
//#define SELECT_POINT	2
//#define DEL_FACE		3
//extern int PickMode = 0;

enum PickMode
{
	None,
	ADD_FACE,
	SELECT_POINT,
	DEL_FACE,
};
PickMode pickMode = None;

using namespace std;

GLuint			program;			// shader program
glm::mat4		proj_matrix;		// projection matrix
MeshObject		model;
ViewManager		camera1, camera2;
GLuint vboPoint;					// vbo for drawing point

glm::vec3 worldPos;
bool updateFlag = false;
bool isRightButtonPress = false;
GLuint currentFaceID = 0;
int currentMouseX = 0;
int currentMouseY = 0;
int OneRingTime = 1;

// checked box 
bool OneRingCheckFace = false;
bool OneRingCheckVertex = false;

float deltaTime = 0.015;
float aspect;
float windowWidth = 509.0;
float windowHeight = 454.0;

// para panel
float aspect2 = 1;
float windowWidth2 = 220.0;
float windowHeight2 = 220.0;
bool drawTexture = false;
float uvRotateAngle = 0.0;
float prevUVRotateAngle = 0.0;

// ****** SHADER ******
DrawModelShader drawModelShader;
DrawPickingFaceShader drawPickingFaceShader;
PickingShader pickingShader;
DrawPointShader drawPointShader;
PickingTexture pickingTexture;


void InitCamera2()
{
	camera2.SetWindowSize(windowWidth2, windowHeight2);
	camera2.ToggleOrtho();
	camera2.SetEyePos(0.0f, 0.0f, -0.10f);
	//camera2.SetEyeLookPos(0.0f, 0.0f, -0.1f);
}

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


void InitData(string filename)
{
	ResourcePath::shaderPath = "./Shader/";
	// ResourcePath::modelPath = "./resource/3DModel/UnionSphere.obj";
	ResourcePath::modelPath = filename;

	//Initialize shaders
	drawModelShader.Init();
	pickingShader.Init();
	drawPickingFaceShader.Init();
	drawPointShader.Init();
	pickingTexture.Init(windowWidth, windowHeight);

	glGenBuffers(1, &vboPoint);

	//Load model to shader program
	My_LoadModel();
	cout << "end of InitData" << endl;

	aspect = windowWidth * 1.0f / windowHeight;
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
	if (pickMode == ADD_FACE || pickMode == DEL_FACE)
	{
		drawPickingFaceShader.Enable();
		drawPickingFaceShader.SetMVMat(value_ptr(mvMat));
		drawPickingFaceShader.SetPMat(value_ptr(pMat));
		model.RenderSelectedFace();
		drawPickingFaceShader.Disable();
	}

	glUseProgram(0);

	// render closest point
	if (pickMode == SELECT_POINT)
	{
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

		glm::vec4 pointColor(0.0, 0.85, 0.0, 1.0);
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

void RenderTexCoordWindow()
{
	float radian = (uvRotateAngle - prevUVRotateAngle) * M_PI / 180.0f;
	glm::mat4 uvRotMat = glm::rotate(radian, glm::vec3(0.0, 0.0, 1.0));
	glm::mat4 mvMat = camera2.GetViewMatrix() * camera2.GetModelMatrix();
	glm::mat4 pMat = camera2.GetProjectionMatrix(aspect2);

	glClearColor(0.3f, 0.3f, 0.3f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 跑了右邊會有圖片
	/*if (drawTexture)
	{
		drawTextureShader.Enable();
		drawTextureShader.SetMVMat(mvMat);
		drawTextureShader.SetPMat(pMat);

		glBindTexture(GL_TEXTURE_2D, textureID);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindTexture(GL_TEXTURE_2D, 0);

		drawTextureShader.Disable();
	}*/

	///////////////////////////////////

	drawModelShader.Enable();

	drawModelShader.SetFaceColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	drawModelShader.SetWireColor(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
	drawModelShader.UseLighting(false);
	drawModelShader.DrawWireframe(true);
	drawModelShader.DrawTexCoord(true);
	drawModelShader.DrawTexture(false);
	drawModelShader.SetMVMat(mvMat);
	drawModelShader.SetPMat(pMat);
	drawModelShader.SetUVRotMat(uvRotMat);

	model.RenderParameterized();

	drawModelShader.Disable();
}

void SelectionHandler(unsigned int x, unsigned int y)
{
	GLuint faceID = pickingTexture.ReadTexture(x, windowHeight - y - 1);
	if (faceID != 0)
	{
		currentFaceID = faceID;
	}

	if (pickMode == ADD_FACE)
	{
		if (faceID != 0)
		{
			model.AddSelectedFace(faceID - 1);

			if (OneRingCheckFace)
			{
				model.SelectOneRing_Face(faceID - 1, OneRingTime, "ADD_FACE");
			}
			else if (OneRingCheckVertex)
				model.SelectOneRing_Vertex(faceID - 1, OneRingTime, "ADD_FACE");
		}
	}
	else if (pickMode == DEL_FACE)
	{
		if (faceID != 0)
		{
			model.DeleteSelectedFace(faceID - 1);

			if (OneRingCheckFace)
			{
				model.SelectOneRing_Face(faceID - 1, OneRingTime, "DEL_FACE");
			}
			else if (OneRingCheckVertex)
				model.SelectOneRing_Vertex(faceID - 1, OneRingTime, "DEL_FACE");
		}
	}
	else if (pickMode == SELECT_POINT)
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