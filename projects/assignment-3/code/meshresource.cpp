#pragma once

#include "config.h"
#include "meshresource.h"
#include "exampleapp.h"
#include <cstring>

MeshResource::MeshResource()
{
	startUp();
}

MeshResource::~MeshResource()
{
}

MeshResource::MeshResource(float* vert, int vertLength, int* indi, int indiLength)		// sets the local buffers
{
	indices = indi;						// used to make EBO
	vertices = vert;					// used to make VBO
	indicesLength = indiLength;
	verticesLength = vertLength;
}
void MeshResource::startUp()
{
	float vert[] =
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

		 0.5f,  0.5f, -0.5f,	0.0f, 0.0f, 0.0f, 1,		0.0f, 0.0f,	//8	// top right
		 0.5f, -0.5f, -0.5f,	0.0f, 1.0f, 0.0f, 1,		0.0f, 1.0f,	//9	// bottom right
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f, 1,		1.0f, 1.0f,	//10// bottom left
		-0.5f,  0.5f, -0.5f,	1.0f, 1.0f, 0.0f, 1,		1.0f, 0.0f	//11// top left
	};
	vertices = vert;
	verticesLength = sizeof(vert);
	int indi[] =
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
	indices = indi;
	indicesLength = sizeof(indi);
}

void MeshResource::bindVertexbuffer()	// bind VBO
{
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*verticesLength, vertices, GL_STATIC_DRAW);
}
void MeshResource::bindIndexBuffer()	// bind EBO
{
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*indicesLength, indices, GL_STATIC_DRAW);
}
void MeshResource::bind()				// bind VAO
{
	glBindVertexArray(VAO);
}
void MeshResource::unBind()				// ubind VAO
{
	glBindVertexArray(0);
}
void MeshResource::bindPointers()		// descrive how to read VBO
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float32) * 9, NULL);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float32) * 9, (GLvoid*)(sizeof(float32) * 3));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float32) * 9, (GLvoid*)(sizeof(float32) * 7));

}
void MeshResource::setup()				// setup the VAO
{
	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);
}

void MeshResource::del()
{
	
}


//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2018 Individual contributors, see AUTHORS file (Benjamin Vesterlund)
//------------------------------------------------------------------------------
