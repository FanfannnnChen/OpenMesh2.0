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

// model panel
float deltaTime = 0.015;
float windowWidth = 582.0;
float windowHeight = 522.0;
float aspect = windowWidth * 1.0f / windowHeight;	// aspect = windowWidth * 1.0f / windowHeight;

// para panel
float windowWidth2			 = 35.0;
float windowHeight2			 = 35.0;
float aspect2				 = windowWidth2 * 1.0f / windowHeight2;
float uvRotateAngle			 = 0.0;
float prevUVRotateAngle		 = 0.0;

// new mesh/ texture
vector<MeshObject> ObjTemp;
vector<int>	ObjTexture;
int PickTextureNum;

// texture
vector<GLuint> textureID(8);
bool drawTexture = false;
bool isMiddleButtonPress = false;	// tuexture moving
vector <string> texture_path;

// ****** SHADER ******
DrawModelShader			drawModelShader;
DrawPickingFaceShader	drawPickingFaceShader;
PickingShader			pickingShader;
DrawPointShader			drawPointShader;
PickingTexture			pickingTexture;

bool Save_File()
{
	int count_ver = 0, count_nor = 0;
	int last_vertex = 0;
	int count = 0, sum = 0;
	vector<int> counting;

	std::ofstream ofs;
	ofs.open("output.obj");
	if (!ofs.is_open()) \
	{
		cout << "Failed to open file.\n";
		return false;						// EXIT_FAILURE
	}
	ofs << "# New OBJ File: ''\nmtllib output.mtl\no originMesh\n";

	//// 算點
	for (int i = 0; i < ObjTemp.size(); i++)
	{
		for (MyMesh::VertexIter v_it = ObjTemp[i].model.mesh.vertices_begin(); v_it != ObjTemp[i].model.mesh.vertices_end(); ++v_it)
		{
			count_ver++;
		}
	}
	// 算normal
	for (int i = 0; i < ObjTemp.size(); i++)
	{
		for (MyMesh::FaceIter f_it = ObjTemp[i].model.mesh.faces_begin(); f_it != ObjTemp[i].model.mesh.faces_end(); ++f_it)
		{
			count_nor++;
		}
	}

	// 原model 塞點資料
	std::vector<MyMesh::Point> vertices;
	vertices.reserve(model.model.mesh.n_vertices() + count_ver);
	for (MyMesh::VertexIter v_it = model.model.mesh.vertices_begin(); v_it != model.model.mesh.vertices_end(); ++v_it)
	{
		vertices.push_back(model.model.mesh.point(*v_it));
		sum++;
		MyMesh::Point p = model.model.mesh.point(*v_it);
	}
	counting.push_back(sum);

	// newMesh 塞點資料
	for (int i = 0; i < ObjTemp.size(); i++)
	{
		for (MyMesh::VertexIter v_it = ObjTemp[i].model.mesh.vertices_begin(); v_it != ObjTemp[i].model.mesh.vertices_end(); ++v_it)
		{
			vertices.push_back(ObjTemp[i].model.mesh.point(*v_it));
			sum++;
			MyMesh::Point p = ObjTemp[i].model.mesh.point(*v_it);
		}
		counting.push_back(sum);
	}

	 //原model 塞點的法向量
	std::vector<MyMesh::Normal> normals;
	normals.reserve(model.model.mesh.n_faces() + count_nor);
	for (MyMesh::FaceIter f_it = model.model.mesh.faces_begin(); f_it != model.model.mesh.faces_end(); ++f_it)
	{
		normals.push_back(model.model.mesh.normal(*f_it));
	}
	//// newMesh 塞點的法向量
	for (int i = 0; i < ObjTemp.size(); i++)
	{
		for (MyMesh::FaceIter f_it = ObjTemp[i].model.mesh.faces_begin(); f_it != ObjTemp[i].model.mesh.faces_end(); ++f_it)
		{
			normals.push_back(ObjTemp[i].model.mesh.normal(*f_it));
		}
	}

	// 寫檔！三個vector進去！
	for (int i = 0; i < vertices.size(); i++)
	{
		ofs << "v " << vertices[i] << endl;
	}

	for (int i = 0; i < ObjTemp.size(); i++)
	{
		// 各個newMesh Obj的vertex textureCoord
		for (MyMesh::VertexIter v_it = ObjTemp[i].model.mesh.vertices_begin(); v_it != ObjTemp[i].model.mesh.vertices_end(); ++v_it)
		{
			ofs << "vt " << ObjTemp[i].model.mesh.texcoord2D(*v_it) << endl;
		}
	}

	for (int i = 0; i < normals.size(); i++)
	{
		ofs << "vn " << normals[i] << endl;
	}

	// 原model faceData
	ofs << "usemtl Default_OBJ.origin\ns off\n";
	for (MyMesh::FaceIter f_it = model.model.mesh.faces_begin(); f_it != model.model.mesh.faces_end(); ++f_it)
	{
		count++;
		ofs << "f ";
		for (MyMesh::FaceVertexIter fv_it = model.model.mesh.fv_iter(*f_it); fv_it.is_valid(); ++fv_it)
		{

			ofs << fv_it->idx() + 1 << "//" << count << " ";
		}
		ofs << endl;
	}

	// newMesh faceData
	for (int i = 0; i < ObjTemp.size(); i++)
	{
		ofs << "usemtl Default_OBJ.0" << i << endl << "s off\n";
		for (MyMesh::FaceIter f_it = ObjTemp[i].model.mesh.faces_begin(); f_it != ObjTemp[i].model.mesh.faces_end(); ++f_it)
		{
			count++;
			ofs << "f ";
			for (MyMesh::FaceVertexIter fv_it = ObjTemp[i].model.mesh.fv_iter(*f_it); fv_it.is_valid(); ++fv_it)
			{

				ofs << fv_it->idx() + 1 + counting[i] << "/" << fv_it->idx() + 1 + (counting[i] - counting[0]) << "/" << count << " ";
			}
			ofs << endl;
		}
	}
	ofs.close();
	return true;
}

bool Save_material()
{
	std::ofstream ofs;
	ofs.open("output.mtl");
	if (!ofs.is_open()) \
	{
		cout << "Failed to open file.\n";
		return false; // EXIT_FAILURE
	}
	ofs << "# New OBJ File: ''\n# Material Count: " << ObjTemp.size() << endl << endl;

	for (int i = 0; i < ObjTemp.size(); i++)
	{
		int textnum;
		int tempID = 0;
		tempID = ObjTexture[i] + 2;
		for (int j = 0; j < 8; j++)
		{
			cout << "ObjTexture:   " << ObjTexture[i] << endl;
			cout << "OUTtextureID:   " << textureID[j] << endl;
			// 存tempObj 
			if (tempID == textureID[j])
			{
				cout << "textureID:   " << textureID[j] << endl;
				textnum = j;
			}
				
		}
		ofs << "newmtl Default_OBJ.0" << i << endl;
		ofs << "Ns 225.000000" << endl
			<< "Ka 1.000000 1.000000 1.000000" << endl
			<< "Kd 1.000000 1.000000 1.000000" << endl
			<< "Ks 0.000000 0.000000 0.000000" << endl
			<< "Ke 0.000000 0.000000 0.000000" << endl
			<< "Ni 1.450000" << endl
			<< "d 1.000000" << endl
			<< "illum 1" << endl
			<< "map_Kd " << texture_path[textnum] << endl << endl;
	}
}

void InitCamera2()
{
	camera2.SetWindowSize(windowWidth2, windowHeight2);
	camera2.ToggleOrtho();
	camera2.SetEyePos(0.0f, 0.0f, -0.10f);
	//camera2.SetEyeLookPos(0.0f, 0.0f, -0.1f);
}

void UpdateScale_from_Panel( int Scale )
{
	cout << Scale << endl;
	ObjTemp.back().UpdateScale(Scale);
}

void Para(float uvRotateAngle = 0 )
{
	prevUVRotateAngle = uvRotateAngle;
	model.Parameterization(uvRotateAngle);
	cout << "------Parameterization finished------" << endl;

	MeshObject tempMesh;
	ObjTemp.push_back(tempMesh);
	ObjTemp.back().Init("newMesh.obj");

	int id = 0;
	while (ObjTemp.back().AddSelectedFace(id))
	{
		id++;
	}
	ObjTemp.back().Parameterization(uvRotateAngle);

	ObjTexture.push_back(PickTextureNum);

	for (int i = 0; i < ObjTemp.size(); i++)
	{
		cout << "i: " << i << "  tid: " << ObjTexture[i] << endl;
	}
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

void My_LoadTextures()
{
	//Texture setting
	///////////////////////////	
	//Load texture data from file
	vector<TextureData> texture_data;
	texture_data.push_back(Common::Load_png((ResourcePath::imagePath + "grass.jpg").c_str()));
	texture_data.push_back(Common::Load_png((ResourcePath::imagePath + "skin.jpg").c_str()));
	texture_data.push_back(Common::Load_png((ResourcePath::imagePath + "pattern.jpg").c_str()));
	texture_data.push_back(Common::Load_png((ResourcePath::imagePath + "eye.jpg").c_str()));
	texture_data.push_back(Common::Load_png((ResourcePath::imagePath + "camouflage.jpg").c_str()));
	texture_data.push_back(Common::Load_png((ResourcePath::imagePath + "green.jpg").c_str()));
	texture_data.push_back(Common::Load_png((ResourcePath::imagePath + "tree.jpg").c_str()));
	texture_data.push_back(Common::Load_png((ResourcePath::imagePath + "flower.jpg").c_str()));

	texture_path.push_back("grass.jpg");
	texture_path.push_back("skin.jpg");
	texture_path.push_back("pattern.jpg");
	texture_path.push_back("eye.jpg");
	texture_path.push_back("camouflage.jpg");
	texture_path.push_back("green.jpg");
	texture_path.push_back("tree.jpg");
	texture_path.push_back("flower.jpg");
	
	for (int i = 0; i < 8; i++)
	{
		cout << "*-*-*-*-*-*-*-*-" << textureID[i] << endl;
		//Generate empty texture
		glGenTextures(1, &textureID[i]);
		glBindTexture(GL_TEXTURE_2D, textureID[i]);

		//Do texture setting
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, texture_data[i].width, texture_data[i].height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data[i].data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
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
	ResourcePath::modelPath = filename;				// modelPath = "./resource/3DModel/UnionSphere.obj";
	ResourcePath::imagePath = "./resource/Imgs/";

	//Initialize shaders
	drawModelShader.Init();
	pickingShader.Init();
	drawPickingFaceShader.Init();
	drawPointShader.Init();
	pickingTexture.Init(windowWidth, windowHeight);

	glGenBuffers(1, &vboPoint);

	//Load model to shader program
	My_LoadModel();
	My_LoadTextures();
	cout << "end of InitData" << endl;

}

void RenderMeshWindow()
{
	//Update shaders' input variable	 
	glm::mat4 mvMat = camera1.GetViewMatrix() * camera1.GetModelMatrix();
	glm::mat4 pMat = camera1.GetProjectionMatrix(aspect);

	// 不畫貼圖 => 跑pick mode
	//if (!drawTexture)
	//{
	//	// 取得 faceID 才能畫
	//	// write faceID+1 to framebuffer
	//	pickingTexture.Enable();
	//
	//	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//
	//	pickingShader.Enable();
	//	pickingShader.SetMVMat(value_ptr(mvMat));
	//	pickingShader.SetPMat(value_ptr(pMat));
	//
	//	model.Render();
	//
	//	pickingShader.Disable();
	//	pickingTexture.Disable();
	//}

	pickingTexture.Enable();

	// draw wire model 都要跑的部分
	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	pickingShader.Enable();
	pickingShader.SetMVMat(value_ptr(mvMat));
	pickingShader.SetPMat(value_ptr(pMat));

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0f, 1.0f);
	model.Render();

	pickingShader.Disable();
	pickingTexture.Disable();

	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawModelShader.Enable();
	glm::mat3 normalMat = glm::transpose(glm::inverse(glm::mat3(mvMat)));

	float radian = uvRotateAngle * M_PI / 180.0f;
	glm::mat4 uvRotMat = glm::rotate(radian, glm::vec3(0.0, 0.0, 1.0));

	drawModelShader.SetWireColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	drawModelShader.SetFaceColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	drawModelShader.UseLighting(true);
	drawModelShader.DrawTexCoord(false);		// 影響texture開關 黑色底
	drawModelShader.DrawTexture(false);			// 影響texture開關 黑色底
	drawModelShader.DrawWireframe(!drawTexture);
	drawModelShader.SetNormalMat(normalMat);
	drawModelShader.SetMVMat(mvMat);
	drawModelShader.SetPMat(pMat);
	drawModelShader.SetUVRotMat(uvRotMat);

	model.Render();
	
#pragma region Pick Mode

	// render selected face
	if (pickMode == ADD_FACE || pickMode == DEL_FACE)
	{
		//drawModelShader.Disable();

		drawPickingFaceShader.Enable();
		drawPickingFaceShader.SetMVMat(value_ptr(mvMat));
		drawPickingFaceShader.SetPMat(value_ptr(pMat));
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.0f, 1.0f);
		model.RenderSelectedFace();
		drawPickingFaceShader.Disable();

		drawModelShader.Disable();
	}

	// render closest point
	if (pickMode == SELECT_POINT)
	{
		//drawModelShader.Disable();
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

		drawModelShader.Disable();
	}
#pragma endregion
	if (drawTexture)
	{
		drawModelShader.DrawTexture(true);

		//glBindTexture(GL_TEXTURE_2D, textureID);
		//model.RenderParameterized();
		
		for (int i = 0; i < ObjTemp.size(); i++)
		{
			// check textureID initial
			if (textureID[ObjTexture[i]] == -1)
			{
				std::cout << "Bind Texture ERROR！！！！" << std::endl;
			}

			glBindTexture(GL_TEXTURE_2D, textureID[ObjTexture[i]]);
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(-1.0f, -1.0f);
			ObjTemp[i].RenderParameterized();

			glBindTexture(GL_TEXTURE_2D, 0);
		}

		// 動態
		glBindTexture(GL_TEXTURE_2D, textureID[PickTextureNum]);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(-1.0f, -1.0f);
		ObjTemp.back().RenderParameterized();
		glBindTexture(GL_TEXTURE_2D, 0);

		//glBindTexture(GL_TEXTURE_2D, 0);
		drawModelShader.Disable();
	}
	glUseProgram(0);
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
	drawModelShader.SetWireColor(glm::vec4(0.8f, 1.0f, 0.3f, 1.0f));
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
		if ( faceID > 0 && faceID < 10201 )
			currentFaceID = faceID;		
	}

	if (pickMode == ADD_FACE)
	{
		if (currentFaceID != 0)
		{
			model.AddSelectedFace(currentFaceID - 1);

			if (OneRingCheckFace)
			{
				model.SelectOneRing_Face(currentFaceID - 1, OneRingTime, "ADD_FACE");
			}
			else if (OneRingCheckVertex)
			{
				model.SelectOneRing_Vertex(currentFaceID - 1, OneRingTime, "ADD_FACE");
			}
				
		}
	}
	else if (pickMode == DEL_FACE)
	{
		if (currentFaceID != 0)
		{
			model.DeleteSelectedFace(currentFaceID - 1);

			if (OneRingCheckFace)
			{
				model.SelectOneRing_Face(currentFaceID - 1, OneRingTime, "DEL_FACE");
			}
			else if (OneRingCheckVertex)
				model.SelectOneRing_Vertex(currentFaceID - 1, OneRingTime, "DEL_FACE");
		}
	}
	else if (pickMode == SELECT_POINT)
	{
		currentMouseX = x;
		currentMouseY = y;
		updateFlag = true;
	}

	if ( isMiddleButtonPress )
	{
		model.ClearAllSelectedFace();
		model.SelectOneRing_Vertex(currentFaceID - 1, OneRingTime, "ADD_FACE");
		model.Parameterization();
		
		ObjTemp.pop_back();
		MeshObject temp;
		ObjTemp.push_back( temp );
		ObjTemp.back().Init("newMesh.obj");
		int id = 0;
		while (ObjTemp.back().AddSelectedFace(id))
		{
			id++;
		}
		ObjTemp.back().Parameterization();
		ObjTexture.push_back(PickTextureNum);
	}
}

// 滑鼠按下
void Mouse_Press(int button, int x, int y)
{
	camera1.mousePressEvent(button, x, y);
	
	// Right Button Actived
	if (button == RIGHT_BUTTON)
	{
		isRightButtonPress = true;
		SelectionHandler(x, y);
	}
	else if (button == MIDDLE_BUTTON)
	{
		isMiddleButtonPress = true;
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
	else if (button == MIDDLE_BUTTON)
	{
		isMiddleButtonPress = false;
	}
}

// 滑鼠移動
void Mouse_Moving(int x, int y)
{
	camera1.mouseMoveEvent(x, y);

	// Right Button Actived ==> Selecting
	if (isRightButtonPress)
	{
		SelectionHandler(x, y);
	}
	else if (isMiddleButtonPress)
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