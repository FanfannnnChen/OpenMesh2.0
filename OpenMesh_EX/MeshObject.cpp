#include "MeshObject.h"
#include <Eigen/Sparse>
#include <map>
#include <algorithm>

#define Quad
//#define Harmonic

struct OpenMesh::VertexHandle const OpenMesh::PolyConnectivity::InvalidVertexHandle;

#pragma region MyMesh

MyMesh::MyMesh()
{
	request_vertex_normals();
	request_vertex_status();
	request_face_status();
	request_edge_status();
}

MyMesh::~MyMesh()
{

}

int MyMesh::FindVertex(MyMesh::Point pointToFind)
{
	int idx = -1;
	for (MyMesh::VertexIter v_it = vertices_begin(); v_it != vertices_end(); ++v_it)
	{
		MyMesh::Point p = point(*v_it);
		if (pointToFind == p)
		{
			idx = v_it->idx();
			break;
		}
	}

	return idx;
}

void MyMesh::ClearMesh()
{
	if (!faces_empty())
	{
		for (MyMesh::FaceIter f_it = faces_begin(); f_it != faces_end(); ++f_it)
		{
			delete_face(*f_it, true);
		}

		garbage_collection();
	}
}

#pragma endregion

#pragma region GLMesh

GLMesh::GLMesh()
{

}

GLMesh::~GLMesh()
{

}

bool GLMesh::Init(std::string fileName)
{
	if (LoadModel(fileName))
	{
		LoadToShader();
		return true;
	}
	return false;
}

void GLMesh::Render()
{
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, mesh.n_faces() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


bool GLMesh::LoadModel(std::string fileName)
{
	OpenMesh::IO::Options ropt;
	if (OpenMesh::IO::read_mesh(mesh, fileName, ropt))
	{
		if (!ropt.check(OpenMesh::IO::Options::VertexNormal) && mesh.has_vertex_normals())
		{
			mesh.request_face_normals();
			mesh.update_normals();
			mesh.release_face_normals();
		}

		return true;
	}

	return false;
}

void GLMesh::LoadToShader()
{
	std::vector<MyMesh::Point> vertices;
	vertices.reserve(mesh.n_vertices());
	for (MyMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
	{
		vertices.push_back(mesh.point(*v_it));

		MyMesh::Point p = mesh.point(*v_it);
	}

	std::vector<MyMesh::Normal> normals;
	normals.reserve(mesh.n_vertices());
	for (MyMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
	{
		normals.push_back(mesh.normal(*v_it));
	}

	std::vector<unsigned int> indices;
	indices.reserve(mesh.n_faces() * 3);
	for (MyMesh::FaceIter f_it = mesh.faces_begin(); f_it != mesh.faces_end(); ++f_it)
	{
		for (MyMesh::FaceVertexIter fv_it = mesh.fv_iter(*f_it); fv_it.is_valid(); ++fv_it)
		{
			indices.push_back(fv_it->idx());
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vboVertices);
	glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MyMesh::Point) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &vboNormal);
	glBindBuffer(GL_ARRAY_BUFFER, vboNormal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MyMesh::Normal) * normals.size(), &normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

#pragma endregion

MeshObject::MeshObject()
{

}

MeshObject::~MeshObject()
{
}

bool MeshObject::Init(std::string fileName)
{
	selectedFace.clear();

	return model.Init(fileName);
}

void MeshObject::Render()
{
	glBindVertexArray(model.vao);
	glDrawElements(GL_TRIANGLES, model.mesh.n_faces() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void MeshObject::RenderSelectedFace()
{
	if (selectedFace.size() > 0)
	{
		std::vector<unsigned int*> offsets(selectedFace.size());
		for (int i = 0; i < offsets.size(); ++i)
		{
			offsets[i] = (GLuint*)(selectedFace[i] * 3 * sizeof(GLuint));
		}

		std::vector<int> count(selectedFace.size(), 3);

		glBindVertexArray(model.vao);
		glMultiDrawElements(GL_TRIANGLES, &count[0], GL_UNSIGNED_INT, (const GLvoid**)&offsets[0], selectedFace.size());
		glBindVertexArray(0);
	}
}

bool MeshObject::AddSelectedFace(unsigned int faceID)
{
	if (std::find(selectedFace.begin(), selectedFace.end(), faceID) == selectedFace.end() &&
		faceID >= 0 && faceID < model.mesh.n_faces())
	{
		selectedFace.push_back(faceID);
		return true;
	}
	return false;
}

void MeshObject::DeleteSelectedFace(unsigned int faceID)
{
	selectedFace.erase(std::remove(selectedFace.begin(), selectedFace.end(), faceID), selectedFace.end());
}

bool MeshObject::FindClosestPoint(unsigned int faceID, glm::vec3 worldPos, glm::vec3& closestPos)
{
	OpenMesh::FaceHandle fh = model.mesh.face_handle(faceID);
	if (!fh.is_valid())
	{
		return false;
	}
	
	double minDistance = 0.0;
	MyMesh::Point p(worldPos.x, worldPos.y, worldPos.z);
	MyMesh::FVIter fv_it = model.mesh.fv_iter(fh);
	MyMesh::VertexHandle closestVH = *fv_it;
	MyMesh::Point v1 = model.mesh.point(*fv_it);
	++fv_it;

	minDistance = (p - v1).norm();
	for (; fv_it.is_valid(); ++fv_it)
	{
		MyMesh::Point v = model.mesh.point(*fv_it);
		double distance = (p - v).norm();
		if (minDistance > distance)
		{
			minDistance = distance;
			closestVH = *fv_it;
		}
	}
	MyMesh::Point closestPoint = model.mesh.point(closestVH);
	closestPos.x = closestPoint[0];
	closestPos.y = closestPoint[1];
	closestPos.z = closestPoint[2];
	return true;
}

void MeshObject::SelectOneRingFace(int faceID, int time)
{
	std::vector< MyMesh::FaceHandle > OneRing;
	int last_size;
	int current_size = 0;

	OneRing.push_back(model.mesh.face_handle(faceID));
	for (int i = 0; i < time; i++)
	{
		last_size = current_size;
		current_size = OneRing.size();

		for (int j = last_size; j < current_size; j++)
		{
			for (MyMesh::FaceFaceIter ff_it = model.mesh.ff_iter(OneRing[j]); ff_it.is_valid(); ++ff_it)
			{
				if (find(OneRing.begin(), OneRing.end(), *ff_it) == OneRing.end())
				{
					OneRing.push_back(*ff_it);
				}
			}
		}
	}
	
	for (int i = 0; i < OneRing.size(); i++)
	{
		AddSelectedFace(OneRing[i].idx());
	}
}

void MeshObject::SelectOneRingVertex(int faceID, int time)
{
	std::vector< MyMesh::FaceHandle > OneRing;
	std::vector< MyMesh::VertexHandle > allPoint;
	int last_size;
	int current_size = 0;
	
	OneRing.push_back(model.mesh.face_handle(faceID));
	// face -> vertex ==> vertext -> face 
	
	for (int i = 0; i < time; i++)
	{
		last_size = current_size;
		current_size = OneRing.size();

		for (int j = last_size; j < current_size; j++)
		{
			for (MyMesh::FaceVertexIter fv_it = model.mesh.fv_iter(OneRing[j]); fv_it.is_valid(); ++fv_it)	// 面找到點
			{	
				if (find(allPoint.begin(), allPoint.end(), *fv_it) == allPoint.end())
				{
					//allPoint.push_back(model.mesh.vertex_handle(fv_it));	// 點都加進去了
					allPoint.push_back(*fv_it);
				}
			}
		}
		

		for (std::vector<MyMesh::VertexHandle>::iterator v_it = allPoint.begin(); v_it != allPoint.end(); v_it++)
		{
			for (MyMesh::VertexFaceIter vf_it = model.mesh.vf_iter(*v_it); vf_it.is_valid(); ++vf_it)
			{
				if (find(OneRing.begin(), OneRing.end(), *vf_it) == OneRing.end())
				{
					OneRing.push_back(*vf_it);
				}
			}
		}
	}
	

	for (int i = 0; i < OneRing.size(); i++)
	{
		AddSelectedFace(OneRing[i].idx());
	}
}

void MeshObject::CreateSubMesh()
{
	if (selectedFace.size() <= 0)
	{
		return;
	}
	
	model.subMesh.ClearMesh();

	std::sort(selectedFace.begin(), selectedFace.end());

	OpenMesh::HPropHandleT<double> heWeight;	// HalfEdge
	OpenMesh::VPropHandleT<int>	row;
	model.subMesh.add_property(heWeight, "heWeight");
	model.subMesh.add_property(row, "row");

	model.subMesh.request_vertex_texcoords2D();
	model.subMesh.request_vertex_normals();
	model.subMesh.request_face_normals();

	std::vector<MyMesh::VertexHandle> VertexHs;
	VertexHs.reserve(3);		// vextor 預留 3 個元素的位置，沒有初始化

	std::map<int, int> UsedVertex;

	// 一個mesh 有很多個面
	for (std::vector< unsigned int >::iterator face_it = selectedFace.begin(); face_it != selectedFace.end(); face_it++)
	{
		MyMesh::FaceHandle Face_h = model.mesh.face_handle(*face_it);

		// 一個面 下去找他的點
		for (MyMesh::FaceVertexIter FV_it = model.mesh.fv_iter(Face_h); FV_it.is_valid(); FV_it++)
		{
			MyMesh::VertexHandle VertexH;
			MyMesh::Point point = model.mesh.point(*FV_it);	// iterator 抓到這個 face 下的點

			// 避免重複儲存點		先去比對used 中有沒有出現過現在當下的點
			std::map<int, int>::iterator UsedVertex_it = UsedVertex.find(FV_it->idx());		// 找vH 

			if (UsedVertex_it == UsedVertex.end())		// vertex 未被儲存過
			{
				VertexH = model.subMesh.add_vertex(point);	// 存點 並回傳此點的handle
				UsedVertex[FV_it->idx()] = VertexH.idx();	// 更新map 此新點已加入過在這個mesh 並存入此點的handle的編號 ( UsedVertex_it->second )

			}
			else
			{
				VertexH = model.subMesh.vertex_handle(UsedVertex_it->second);
			}

			VertexHs.push_back(VertexH);	// 把這些vertex handle 存起來
		}
		model.subMesh.add_face(VertexHs);	// 建成 Face
		VertexHs.clear();

	}
	model.subMesh.update_normals();

	// output
	for (MyMesh::FaceIter fit = model.subMesh.faces_begin(); fit != model.subMesh.faces_end(); fit++)
	{
		std::cout << fit->idx() << std::endl;
	}

	// debug
	OpenMesh::IO::Options wopt;
	wopt += OpenMesh::IO::Options::VertexTexCoord;
	wopt += OpenMesh::IO::Options::VertexNormal;

	if (!OpenMesh::IO::write_mesh(model.subMesh, "debug.obj", wopt))
	{
		printf("Write Mesh Error\n");
	}
}