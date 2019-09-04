#pragma once
#include "meshresource.h"


class Renderer
{
public:
	Renderer();
	~Renderer();

	void addBuffers();

private:

	vector<Vertex> vBuffer;
	vector<unsigned int> iBuffer;
	MeshResource mesh;
};

inline Renderer::Renderer()
{

}

inline Renderer::~Renderer()
{

}

inline void Renderer::addBuffers()
{
	mesh.loadOBJ("Obj/triangle.obj");
	vBuffer = mesh.getVBuffer();
	iBuffer = mesh.getIBuffer();
}
