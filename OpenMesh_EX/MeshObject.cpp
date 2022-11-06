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

void GLMesh::LoadTexCoordToShader()
{
	if (mesh.has_vertex_texcoords2D())
	{
		std::vector<MyMesh::TexCoord2D> texCoords;
		for (MyMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
		{
			MyMesh::TexCoord2D texCoord = mesh.texcoord2D(*v_it);
			texCoords.push_back(texCoord);
		}

		glBindVertexArray(vao);

		glGenBuffers(1, &vboTexCoord);
		glBindBuffer(GL_ARRAY_BUFFER, vboTexCoord);
		glBufferData(GL_ARRAY_BUFFER, sizeof(MyMesh::TexCoord2D) * texCoords.size(), &texCoords[0], GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
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

void MeshObject::SelectOneRing_Face(int faceID, int time, std::string pickMode)
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
	
	if (pickMode == "ADD_FACE")
	{
		for (int i = 0; i < OneRing.size(); i++)
		{
			AddSelectedFace(OneRing[i].idx());
		}
	}
	else if (pickMode == "DEL_FACE")
	{
		for (int i = 0; i < OneRing.size(); i++)
		{
			DeleteSelectedFace(OneRing[i].idx());
		}
	}
}

void MeshObject::SelectOneRing_Vertex(int faceID, int time, std::string pickMode)
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
	
	if (pickMode == "ADD_FACE")
	{
		for (int i = 0; i < OneRing.size(); i++)
		{
			AddSelectedFace(OneRing[i].idx());
		}
	}
	else if (pickMode == "DEL_FACE")
	{
		for (int i = 0; i < OneRing.size(); i++)
		{
			DeleteSelectedFace(OneRing[i].idx());
		}
	}
	
}

void MeshObject::CreateNewMesh(MyMesh& mesh)
{
	mesh.request_vertex_texcoords2D();
	mesh.request_vertex_normals();
	mesh.request_face_normals();

	std::vector<MyMesh::VertexHandle> VertexHs;
	VertexHs.reserve(3);		// vextor 預留 3 個元素的位置，沒有初始化

	std::map<int, int> usedVertex;

	// 一個mesh 有很多個面
	for (std::vector<unsigned int>::iterator f_it = selectedFace.begin(); f_it != selectedFace.end(); ++f_it)
	{
		MyMesh::FaceHandle fh = model.mesh.face_handle(*f_it);

		// 一個面 下去找他的點
		for (MyMesh::FaceVertexIter fv_it = model.mesh.fv_iter(fh); fv_it.is_valid(); ++fv_it)
		{
			MyMesh::VertexHandle vh;
			MyMesh::Point p = model.mesh.point(*fv_it);		// iterator 抓到這個 face 下的點

			// 避免重複儲存點		先去比對used 中有沒有出現過現在當下的點
			std::map<int, int>::iterator usedVertex_it = usedVertex.find(fv_it->idx());

			if (usedVertex_it == usedVertex.end())		// vertex 未被儲存過
			{
				vh = mesh.add_vertex(p);				// 存點 並回傳此點的handle
				usedVertex[fv_it->idx()] = vh.idx();	// 更新map 此新點已加入過在這個mesh 並存入此點的handle的編號 ( UsedVertex_it->second )
			}
			else
			{
				vh = mesh.vertex_handle(usedVertex_it->second);
			}

			VertexHs.push_back(vh);		// 把這些vertex handle 存起來
		}

		mesh.add_face(VertexHs);		// 建成 Face
		VertexHs.clear();
	}

	mesh.update_normals();
}

void MeshObject::Parameterization(float uvRotateAngle)
{
	if (selectedFace.size() <= 0)
	{
		return;
	}

	std::sort(selectedFace.begin(), selectedFace.end());
	
	OpenMesh::HPropHandleT<double> heWeight;		// Half Edge Weight
	OpenMesh::VPropHandleT<int> row;
	MyMesh mesh;
	mesh.add_property(heWeight, "heWeight");
	mesh.add_property(row, "row");

	CreateNewMesh(mesh);

	//calculate weight
	MyMesh::HalfedgeHandle half_edge_h;
	for (MyMesh::EdgeIter e_it = mesh.edges_begin(); e_it != mesh.edges_end(); ++e_it)
	{
		if (!mesh.is_boundary(*e_it))	// 若這個邊不是邊界才進入
		{
			GLdouble angle1, angle2, w;
			MyMesh::HalfedgeHandle _heh = mesh.halfedge_handle(*e_it, 0);	// 默認這個
			MyMesh::Point pFrom = mesh.point(mesh.from_vertex_handle(_heh));
			MyMesh::Point pTo = mesh.point(mesh.to_vertex_handle(_heh));
			MyMesh::Point p1 = mesh.point(mesh.opposite_vh(_heh));
			MyMesh::Point p2 = mesh.point(mesh.opposite_he_opposite_vh(_heh));


			double edgeLen = (pFrom - pTo).length();

			// weight from to
			OpenMesh::Vec3d v1 = (OpenMesh::Vec3d)(pTo - pFrom);
			v1.normalize();

			OpenMesh::Vec3d v2 = (OpenMesh::Vec3d)(p1 - pFrom);
			v2.normalize();

			angle1 = std::acos(OpenMesh::dot(v1, v2));

			v2 = (OpenMesh::Vec3d)(p2 - pFrom);
			v2.normalize();

			angle2 = std::acos(OpenMesh::dot(v1, v2));

			w = (std::tan(angle1 / 2.0f) + std::tan(angle2 / 2.0f)) / edgeLen;

			mesh.property(heWeight, _heh) = w;

			// weight to from
			v1 = -v1;

			v2 = (OpenMesh::Vec3d)(p1 - pTo);
			v2.normalize();

			angle1 = std::acos(OpenMesh::dot(v1, v2));

			v2 = (OpenMesh::Vec3d)(p2 - pTo);
			v2.normalize();

			angle2 = std::acos(OpenMesh::dot(v1, v2));

			w = (std::tan(angle1 / 2.0f) + std::tan(angle2 / 2.0f)) / edgeLen;

			mesh.property(heWeight, mesh.opposite_halfedge_handle(_heh)) = w;

		}
		else
		{
			if (!half_edge_h.is_valid())
			{
				half_edge_h = mesh.halfedge_handle(*e_it, 1);
			}
		}
	}

	// calculate matrix size
	int count = 0;
	for (MyMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
	{
		if (mesh.is_boundary(*v_it))
		{
			mesh.property(row, *v_it) = -1;
		}
		else
		{
			mesh.property(row, *v_it) = count++;
		}
	}

	// calculate perimeter
	double perimeter = 0;
	std::vector<double> segLength;
	std::vector<MyMesh::VertexHandle> vhs;
	MyMesh::HalfedgeHandle half_edge_h_Next = half_edge_h;
	do
	{
		MyMesh::Point from = mesh.point(mesh.from_vertex_handle(half_edge_h_Next));
		MyMesh::Point to = mesh.point(mesh.to_vertex_handle(half_edge_h_Next));
		perimeter += (from - to).length();

		segLength.push_back(perimeter);
		vhs.push_back(mesh.from_vertex_handle(half_edge_h_Next));

		half_edge_h_Next = mesh.next_halfedge_handle(half_edge_h_Next);
	} while (half_edge_h != half_edge_h_Next);


	float rd = (225 + uvRotateAngle) * M_PI / 180.0;
	float initDist = 0;
	MyMesh::TexCoord2D st(0, 0);
	float R = std::sqrt(2) / 2.0;
	st[0] = R * cos(rd) + 0.5;
	st[1] = R * sin(rd) + 0.5;

	if (st[0] > 1)
	{
		st[0] = 1;
		st[1] = tan(rd) / 2 + 0.5;
	}

	if (st[0] < 0)
	{
		st[0] = 0;
		st[1] = 0.5 - tan(rd) / 2;
	}

	if (st[1] > 1)
	{
		st[0] = tan(M_PI_2 - rd) / 2 + 0.5;
		st[1] = 1;
	}

	if (st[1] < 0)
	{
		st[0] = 0.5 - tan(M_PI_2 - rd) / 2;
		st[1] = 0;
	}


	if (uvRotateAngle <= 90)
	{
		initDist = st.length();
	}

	else if (uvRotateAngle <= 180)
	{
		initDist = 1 + st[1];
	}

	else if (uvRotateAngle <= 270)
	{
		initDist = 3 - st[0];
	}

	else
	{
		initDist = 4 - st[1];
	}

	mesh.set_texcoord2D(vhs[0], st);
	perimeter /= 4.0;
	for (int i = 1; i < vhs.size(); ++i)
	{
		double curLen = segLength[i - 1] / perimeter + initDist;
		if (curLen > 4)
		{
			curLen -= 4;
		}

		if (curLen <= 1)
		{
			st[0] = curLen;
			st[1] = 0;
		}
		else if (curLen <= 2)
		{
			st[0] = 1;
			st[1] = curLen - 1;
		}
		else if (curLen <= 3)
		{
			st[0] = 3 - curLen;
			st[1] = 1;
		}
		else
		{
			st[0] = 0;
			st[1] = 4 - curLen;
		}

		mesh.set_texcoord2D(vhs[i], st);
	}

	typedef Eigen::SparseMatrix<double> SpMat;

	SpMat A(count, count);
	Eigen::VectorXd BX(count);
	Eigen::VectorXd BY(count);
	Eigen::SimplicialLDLT<Eigen::SparseMatrix<double> > linearSolver;

	BX.setZero();
	BY.setZero();

	// fiil matrix
	for (MyMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
	{
		if (!mesh.is_boundary(*v_it))
		{
			int i = mesh.property(row, *v_it);
			double totalWeight = 0;

			for (MyMesh::VertexVertexIter vv_it = mesh.vv_iter(*v_it); vv_it.is_valid(); ++vv_it)
			{
				MyMesh::HalfedgeHandle _heh = mesh.find_halfedge(*v_it, *vv_it);
				double w = mesh.property(heWeight, _heh);

				if (mesh.is_boundary(*vv_it))
				{
					MyMesh::TexCoord2D texCoord = mesh.texcoord2D(*vv_it);
					BX[i] += w * texCoord[0];
					BY[i] += w * texCoord[1];
				}
				else
				{
					int j = mesh.property(row, *vv_it);
					A.insert(i, j) = -w;
				}
				totalWeight += w;
			}


			A.insert(i, i) = totalWeight;
		}
	}

	A.makeCompressed();

	// solve linear system
	SpMat At = A.transpose();
	linearSolver.compute(At * A);

	Eigen::VectorXd TX = linearSolver.solve(At * BX);
	Eigen::VectorXd TY = linearSolver.solve(At * BY);

	// set texcoord
	for (MyMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
	{
		if (!mesh.is_boundary(*v_it))
		{
			int i = mesh.property(row, *v_it);
			mesh.set_texcoord2D(*v_it, MyMesh::TexCoord2D(TX[i], TY[i]));
		}
	}

	// request vertex texcoord, if not exist 
	if (!model.mesh.has_vertex_texcoords2D())
	{
		model.mesh.request_vertex_texcoords2D();
		for (MyMesh::VertexIter v_it = model.mesh.vertices_begin(); v_it != model.mesh.vertices_end(); ++v_it)
		{
			model.mesh.set_texcoord2D(*v_it, MyMesh::TexCoord2D(-1, -1));
		}
	}

	// map texcoord back to origin mesh
	int index = 0;
	for (MyMesh::FaceIter f_it = mesh.faces_begin(); f_it != mesh.faces_end(); ++f_it)
	{
		MyMesh::FaceHandle fh = *f_it;
		MyMesh::FaceHandle selectedFace_h = model.mesh.face_handle(selectedFace[index++]);

		MyMesh::FaceVertexIter fv_it = mesh.fv_iter(fh);
		MyMesh::FaceVertexIter selectedfv_it = model.mesh.fv_iter(selectedFace_h);
		for (; fv_it.is_valid() && selectedfv_it.is_valid(); ++fv_it, ++selectedfv_it)
		{
			MyMesh::TexCoord2D texCoord = mesh.texcoord2D(*fv_it);
			model.mesh.set_texcoord2D(*selectedfv_it, texCoord);
		}
	}

	model.LoadTexCoordToShader();

	fvIDsPtr.swap(std::vector<unsigned int*>(selectedFace.size()));
	for (int i = 0; i < fvIDsPtr.size(); ++i)
	{
		fvIDsPtr[i] = (GLuint*)(selectedFace[i] * 3 * sizeof(GLuint));
	}
	elemCount.swap(std::vector<int>(selectedFace.size(), 3));

	SaveNewMesh(mesh, "newMesh.obj");

	// debug
	/*OpenMesh::IO::Options wopt;
	wopt += OpenMesh::IO::Options::VertexTexCoord;
	wopt += OpenMesh::IO::Options::VertexNormal;

	if (!OpenMesh::IO::write_mesh(mesh, "debug.obj", wopt))
	{
		printf("Write Mesh Error\n");
	}*/
}

void MeshObject::RenderParameterized()
{
	if (model.mesh.has_vertex_texcoords2D())
	{
		glBindVertexArray(model.vao);
		glMultiDrawElements(GL_TRIANGLES, &elemCount[0], GL_UNSIGNED_INT, (const GLvoid**)&fvIDsPtr[0], elemCount.size());
		glBindVertexArray(0);
	}
}

void MeshObject::CreateLoadNewMesh(MeshObject& NewMesh)
{
	if (selectedFace.size() <= 0)
	{
		return;
	}

	std::sort(selectedFace.begin(), selectedFace.end());

	MyMesh mesh;
	CreateNewMesh(mesh);

	SaveNewMesh(mesh, "newMesh.obj");

	if (NewMesh.Init("newMesh.obj"))
	{
		std::cout << "New Mesh initial" << std::endl;
	}
	else 
		std::cout << "New Mesh initial failed" << std::endl;
}

void MeshObject::ClearAllSelectedFace()
{
	selectedFace.clear();
}

void MeshObject::SaveNewMesh( MyMesh& mesh, std::string _fileName)
{
	// debug
	OpenMesh::IO::Options wopt;
	wopt += OpenMesh::IO::Options::VertexTexCoord;
	wopt += OpenMesh::IO::Options::VertexNormal;

	if (!OpenMesh::IO::write_mesh(mesh, _fileName, wopt))
	{
		printf("Write Mesh Error\n");
	}
}
