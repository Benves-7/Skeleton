#pragma once
#include "meshresource.h"
#include "textureresource.h"
#include "ShaderObject.h"
#include "Camera.h"
#include <memory>

class GraphicsNode
{
public:

	Camera camera;

	GraphicsNode();
	~GraphicsNode();

	void setMeshClass(std::shared_ptr<MeshResource> meshResource);
	MeshResource* getMeshClass();
	void setTextureResource(std::shared_ptr<TextureResource> textureResource);
	TextureResource* getTextureClass();
	void setShaderObject(std::shared_ptr<ShaderObject> shaderObject);
	ShaderObject* getShaderClass();

	void setup(const std::string& vertexShaderFile, const std::string& fragmentShaderFile, const std::string& textureFile);
	void draw();


private:

	std::shared_ptr<MeshResource> mesh;
	std::shared_ptr<TextureResource> texture;
	std::shared_ptr<ShaderObject> shader;
	
	Matrix4D transform;

};

inline GraphicsNode::GraphicsNode()
{

}
inline GraphicsNode::~GraphicsNode()
{

}
/// binds the shared pointer
inline void GraphicsNode::setMeshClass(std::shared_ptr<MeshResource> meshResource)
{
	this->mesh = meshResource;
}
/// Returns the bound shared pointer
inline MeshResource* GraphicsNode::getMeshClass()
{
	return mesh.get();
}
/// binds the shared pointer
inline void GraphicsNode::setTextureResource(std::shared_ptr<TextureResource> textureResource)
{
	this->texture = textureResource;
}
/// Returns the bound shared pointer
inline TextureResource* GraphicsNode::getTextureClass()
{
	return texture.get();
}
/// binds the shared pointer
inline void GraphicsNode::setShaderObject(std::shared_ptr<ShaderObject> shaderObject)
{
	this->shader = shaderObject;
}
/// Returns the bound shared pointer
inline ShaderObject* GraphicsNode::getShaderClass()
{
	return shader.get();
}
/// sets all the code only needed to compute once.
inline void GraphicsNode::setup(const std::string& vertexShaderFile, const std::string& fragmentShaderFile, const std::string& textureFile)
{
	mesh->setup();					//VAO
	mesh->bindVertexbuffer();		//VBO
	mesh->bindIndexBuffer();		//EBO
	mesh->bindPointers();			//read protocols
	camera.setup();
	shader->loadShaders(vertexShaderFile, fragmentShaderFile);
	texture->makeTexture("Texture/container.jpg");
	texture->bindTexture();

}
///runs all the code needed to update every loop of the program.
inline void GraphicsNode::draw()
{
	shader->useProgram();
	mesh->bind();
	shader->modifyVertexShader(camera.run()* mesh->getModelMatrix());
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	mesh->unBind();
}