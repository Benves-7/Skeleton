#pragma once
#include <core/app.h>
#include <string.h>
#include <vector>
#include <map>
#include "MathLib.h"

#pragma region MeshResource
using namespace std;
struct FaceVertex
{
	string str;
	unsigned int pos;
	unsigned int uv;
	unsigned int normal;
};
struct Vertex
{
	float pos[3];
	float uv[2];
	float normal[3];
};
class MeshResource
{
public:
	MeshResource();
	MeshResource(float* vert, int vertsize, int* i, int isize);
	~MeshResource();

	void bindVertexbuffer();
	void bindIndexBuffer();

	void bind();
	void unBind();

	void bindPointers();
	void setup();
	
	/// Returns modelmatrix. used to move, rotate and scale the object.
	inline Matrix4D getModelMatrix() { return translation * rotation * scale; }
	/// Makes a Translate matrix.	(move)
	inline void translate(Vector4D vec)
	{
		translation = Matrix4D(
			1, 0, 0, vec.GetX(),
			0, 1, 0, vec.GetY(),
			0, 0, 1, vec.GetZ(),
			0, 0, 0, 1);
	}
	/// Makes a Scaling matrix.		(size)
	inline void scaling(Vector4D vec)
	{
		scale = Matrix4D(
			vec.GetX(), 0, 0, 0,
			0, vec.GetY(), 0, 0,
			0, 0, vec.GetZ(), 0,
			0, 0, 0, 1);
	}
	/// Makes a Rotation matrix		(rotate)
	inline void rotate(Vector4D vec, float radians)
	{
	    // mvh fredrik
	    for (int i = 0; i < 1000; i++)
	        printf(".");
		rotation = Matrix4D::RotVec(vec, radians);
	}

	inline int getVBufferLength()
	{
		return vBufferLength;
	}
	inline int getIBufferLength()
	{
		return iBufferLength;
	}
	inline int getIBufferElements()
	{
		return iBufferelements;
	}

	inline void loadOBJ(const char* filename)
	{
		if (!loadOBJinternal(filename, vbuffer, ibuffer))
		{
			printf("Failed to load OBJ\n");
		}
		else
		{
			iBufferelements = ibuffer.size();
			iBufferLength  = ibuffer.size() * sizeof(float);
			vBufferLength = vbuffer.size() * sizeof(Vertex);
		}
	}

	void makeVertexBuffer();


	unsigned int VBO;		// vertex buffer object
	unsigned int VAO;		// vertex array object
	unsigned int EBO;		// element buffer object


private:

	int iBufferelements;
	int iBufferLength;
	int vBufferLength;
	int check = sizeof(float);

	Matrix4D translation;
	Matrix4D scale;
	Matrix4D rotation;

	// Functions
	bool loadOBJinternal(const char* path, vector<Vertex> &vbuffer, vector<unsigned int> &ibuffer);

	// Read our .obj file
	vector<Vertex> vbuffer;
	vector<unsigned int> ibuffer;

	vector<Vector4D> verticesVec;
	vector<Vector2D> uvs;
	vector<Vector4D> normals; // Won't be used at the moment.
};

/// Default constructor
inline MeshResource::MeshResource()
{
	translation = Matrix4D();
	scale = Matrix4D();
	rotation = Matrix4D();
}
/// Constructor to make a diffrent vertices and indices the the default once.
inline MeshResource::MeshResource(float* vert, int vertsize, int* i, int isize)
{
}
inline MeshResource::~MeshResource()
{
	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VAO);

}
/// Makes VBO.
inline void MeshResource::bindVertexbuffer()
{
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vBufferLength, &vbuffer[0], GL_STATIC_DRAW);
}
/// Makes EBO.
inline void MeshResource::bindIndexBuffer()
{
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, iBufferLength, &ibuffer[0], GL_STATIC_DRAW);
}
/// Bind functions to bind and unbind VAO.
inline void MeshResource::bind()
{
	glBindVertexArray(VAO);
}
inline void MeshResource::unBind()
{
	glBindVertexArray(0);
}
/// Binds the Vertex Attrib Array (the instruction on how to read the VBO,EBO,VAO).
inline void MeshResource::bindPointers()
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float32) * 8 , (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float32) * 8, (GLvoid*)(sizeof(float32) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float32) * 8, (GLvoid*)(sizeof(float32) * 5));

}
/// Sets the VAO.
inline void MeshResource::setup()
{
	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);
}
/// Loads in the OBJ file.
inline bool MeshResource::loadOBJinternal(const char* path, vector<Vertex> &vbuffer, vector<unsigned int> &ibuffer)
{
	vector<unsigned int> vertexIndices, textureIndices, normalIndices;
	vector<FaceVertex> faceList;
	map<string, unsigned int> indices;

	vector<Vector4D> temp_Vertices;
	vector<Vector2D> temp_Texture;
	vector<Vector4D> temp_Normals;

	int matches;
	unsigned int rows_read = 0;

	FILE* file = fopen(path, "r");
	if (file == NULL)
	{
		printf("File not found! \n");
		return false;
	}
	while (true)
	{

		char lineHeader[255];
		// read the first word
		int res = fscanf(file, "%s\n", lineHeader);
		if (res == EOF) // EOF = End Of File.
			break;
		if (strcmp(lineHeader, "v") == 0) 
		{
			Vector4D vertex;
			fscanf(file, "%f %f %f\n", &vertex[0], &vertex[1], &vertex[2]);
			temp_Vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vn") == 0)
		{
			Vector4D normal;
			fscanf(file, "%f %f %f\n", &normal[0], &normal[1], &normal[2]);
			temp_Normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "vt") == 0)
		{
			Vector2D uv;
			float trow;
			fscanf(file, "%f %f %f\n", &uv[0], &uv[1], &trow);
			temp_Texture.push_back(uv);
		}
		else if (strcmp(lineHeader, "f") == 0)
		{
			unsigned int vertexIndex[4], uvIndex[4], normalIndex[4];
			matches = fscanf(
			file, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n",
			&vertexIndex[0], &uvIndex[0], &normalIndex[0],
			&vertexIndex[1], &uvIndex[1], &normalIndex[1],
			&vertexIndex[2], &uvIndex[2], &normalIndex[2],
			&vertexIndex[3], &uvIndex[3], &normalIndex[3]);
			if (matches == 9)
			{
				FaceVertex face_[3];

				string str0 = string(to_string(vertexIndex[0]) + "/" + to_string(uvIndex[0]) + "/" + to_string(normalIndex[0]));
				string str1 = string(to_string(vertexIndex[1]) + "/" + to_string(uvIndex[1]) + "/" + to_string(normalIndex[1]));
				string str2 = string(to_string(vertexIndex[2]) + "/" + to_string(uvIndex[2]) + "/" + to_string(normalIndex[2]));

				face_[0].str = str0;
				face_[0].pos = vertexIndex[0];
				face_[0].uv = uvIndex[0];
				face_[0].normal = normalIndex[0];

				face_[1].str = str1;
				face_[1].pos = vertexIndex[1];
				face_[1].uv = uvIndex[1];
				face_[1].normal = normalIndex[1];

				face_[2].str = str2;
				face_[2].pos = vertexIndex[2];
				face_[2].uv = uvIndex[2];
				face_[2].normal = normalIndex[2];

				faceList.push_back(face_[0]);
				faceList.push_back(face_[1]);
				faceList.push_back(face_[2]);
			}
			else if (matches == 12)
			{
				FaceVertex face[4];
				string str0 = string(to_string(vertexIndex[0]) + "/" + to_string(uvIndex[0]) + "/" + to_string(normalIndex[0]));
				string str1 = string(to_string(vertexIndex[1]) + "/" + to_string(uvIndex[1]) + "/" + to_string(normalIndex[1]));
				string str2 = string(to_string(vertexIndex[2]) + "/" + to_string(uvIndex[2]) + "/" + to_string(normalIndex[2]));
				string str3 = string(to_string(vertexIndex[3]) + "/" + to_string(uvIndex[3]) + "/" + to_string(normalIndex[3]));

				face[0].str = str0;
				face[0].pos = vertexIndex[0];
				face[0].uv = uvIndex[0];
				face[0].normal = normalIndex[0];

				face[1].str = str1;
				face[1].pos = vertexIndex[1];
				face[1].uv = uvIndex[1];
				face[1].normal = normalIndex[1];

				face[2].str = str2;
				face[2].pos = vertexIndex[2];
				face[2].uv = uvIndex[2];
				face[2].normal = normalIndex[2];

				face[3].str = str3;
				face[3].pos = vertexIndex[3];
				face[3].uv = uvIndex[3];
				face[3].normal = normalIndex[3];

				faceList.push_back(face[0]);
				faceList.push_back(face[1]);
				faceList.push_back(face[2]);

				faceList.push_back(face[0]);
				faceList.push_back(face[2]);
				faceList.push_back(face[3]);
			}
			else 
			{
				printf("File can't be read");
				return false;
			}
		}
	}
	printf("Done reading! \n");
    for (FaceVertex const& face : faceList)
	{
		if (indices.count(face.str))
		{
			ibuffer.push_back(indices[face.str]);
			continue;
		}
		Vertex v;

		v.pos[0] = temp_Vertices[face.pos - 1].GetX();
		v.pos[1] = temp_Vertices[face.pos - 1].GetY();
		v.pos[2] = temp_Vertices[face.pos - 1].GetZ();

		v.uv[0] = 0;//temp_Texture[face.uv - 1].GetX();
		v.uv[1] = 0;// temp_Texture[face.uv - 1].GetY();

		v.normal[0] = temp_Normals[face.normal - 1].GetX();
		v.normal[1] = temp_Normals[face.normal - 1].GetY();
		v.normal[2] = temp_Normals[face.normal - 1].GetZ();

		vbuffer.push_back(v);
		unsigned int index = vbuffer.size() - 1;
		ibuffer.push_back(index);
		indices.insert(make_pair(face.str, index));
	}
	printf("done with vertex \n");

    return true;

}
#pragma endregion