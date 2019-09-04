#pragma once
#pragma once
#include <core/app.h>
#pragma region MeshResource

class MeshResource
{
public:
	MeshResource();
	MeshResource(float* vert, int vertLength, int* indi, int indiLength);
	~MeshResource();

	void startUp();
	void bindVertexbuffer();
	void bindIndexBuffer();
	void bind();
	void unBind();

	void bindPointers();
	void setup();
	void del();


	unsigned int VBO;		// vertex buffer object
	unsigned int VAO;		// vertex array object
	unsigned int EBO;		// element buffer object
	int* indices;
	float* vertices;
	int indicesLength;
	int verticesLength;

private:

};
#pragma endregion