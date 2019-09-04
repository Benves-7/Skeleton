#pragma once
#include <core/app.h>
#include "MathLib.h"

#pragma region MeshResource
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
		rotation = Matrix4D::RotVec(vec, radians);
	}

	unsigned int VBO;		// vertex buffer object
	unsigned int VAO;		// vertex array object
	unsigned int EBO;		// element buffer object


private:

	int* indices;
	float* vertices;
	int indicesLength;
	int verticesLength;
	int check = sizeof(float);

	Matrix4D translation;
	Matrix4D scale;
	Matrix4D rotation;

	float verti[108] =
	{
		// positions			// colors					// texture coords
		 0.5f,  0.5f, 0.5f,		1.0f, 0.0f, 0.0f, 1,		1.0f, 1.0f,	//0	// top right
		 0.5f, -0.5f, 0.5f,		0.0f, 1.0f, 0.0f, 1,		1.0f, 0.0f,	//1	// bottom right
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f, 1,		0.0f, 0.0f,	//2	// bottom left
		-0.5f,  0.5f, 0.5f,		1.0f, 1.0f, 0.0f, 1,		0.0f, 1.0f,	//3	// top left

		 0.5f,  0.5f, -0.5f,	1.0f, 0.0f, 0.0f, 1,		0.0f, 1.0f,	//4	// top right
		 0.5f, -0.5f, -0.5f,	0.0f, 1.0f, 0.0f, 1,		0.0f, 0.0f,	//5	// bottom right
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f, 1,		1.0f, 0.0f,	//6	// bottom left
		-0.5f,  0.5f, -0.5f,	1.0f, 1.0f, 0.0f, 1,		1.0f, 1.0f,	//7	// top left

		 0.5f,  0.5f, -0.5f,	0.0f, 0.0f, 0.0f, 1,		1.0f, 0.0f,	//8	// top right
		 0.5f, -0.5f, -0.5f,	0.0f, 1.0f, 0.0f, 1,		1.0f, 1.0f,	//9	// bottom right
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f, 1,		0.0f, 1.0f,	//10// bottom left
		-0.5f,  0.5f, -0.5f,	1.0f, 1.0f, 0.0f, 1,		0.0f, 0.0f	//11// top left

	};
	int indi[36] =
	{
		0,1,3,		//first triangle		Front
		1,2,3,		//second triangle

		4,5,7,		//third triangle		Back
		5,6,7,		//fourth triangle

		0,5,4,		//fifth triangle		Left
		0,1,5,		//sixth triangle

		3,7,2,		//seventh triangle		Right
		2,7,6,		//eigth triangle

		0,8,11,		//nineth triangle		Top
		0,11,3,		//tenth triangle

		1,2,9,		//eleventh triangle		Bottom
		2,10,9		//twelth triangle
	};




};

/// Default constructor
inline MeshResource::MeshResource()
{
	vertices = verti;
	verticesLength = sizeof(verti);
	indices = indi;
	indicesLength = sizeof(indi);
}
/// Constructor to make a diffrent vertices and indices the the default once.
inline MeshResource::MeshResource(float* vert, int vertsize, int* i, int isize)
{
	vertices = vert;
	verticesLength = vertsize;
	indices = i;
	indicesLength = isize;
}
inline MeshResource::~MeshResource()
{
}
/// Makes VBO.
inline void MeshResource::bindVertexbuffer()
{
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verticesLength, vertices, GL_STATIC_DRAW);
}
/// Makes EBO.
inline void MeshResource::bindIndexBuffer()
{
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesLength, indices, GL_STATIC_DRAW);
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float32) * 9, NULL);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float32) * 9, (GLvoid*)(sizeof(float32) * 3));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float32) * 9, (GLvoid*)(sizeof(float32) * 7));

}
/// Sets the VAO.
inline void MeshResource::setup()
{
	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);
}
/// Makes a rotation Matrix if object is to move.
#pragma endregion