#pragma once
#include "light.h"
#include "meshresource.h"
#include "textureresource.h"
#include "ShaderObject.h"
#include "Camera.h"
#include <memory>

class GraphicsNode
{
public:

	GraphicsNode();
	~GraphicsNode();

	void setMeshClass(std::shared_ptr<MeshResource> meshResource);
	MeshResource* getMeshClass();
	void setTextureResource(std::shared_ptr<TextureResource> textureResource);
	TextureResource* getTextureClass();
	void setShaderObject(std::shared_ptr<ShaderObject> shaderObject);
	ShaderObject* getShaderClass();
	void setLightNode(std::shared_ptr<LightNode> lightNode);
	LightNode* getLightClass();

	void draw();
	void setCamera(Camera in_camera)
	{
		cam = in_camera;
	}

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

private:

	std::shared_ptr<LightNode> light;
	std::shared_ptr<MeshResource> mesh;
	std::shared_ptr<TextureResource> texture;
	std::shared_ptr<ShaderObject> shader;

	Matrix4D translation;
	Matrix4D scale;
	Matrix4D rotation;
	
	Matrix4D transform;
	Camera cam;

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

inline void GraphicsNode::setLightNode(std::shared_ptr<LightNode> lightNode)
{
	this->light = lightNode;
}

inline LightNode* GraphicsNode::getLightClass()
{
	return  light.get();
}
///runs all the code needed to update every loop of the program.
inline void GraphicsNode::draw()
{
	shader->useProgram();
	mesh->bind();
	shader->modifyVertexShader("transform", cam.run());
	shader->modifyVertexShader("model", getModelMatrix());

	shader->modifyFragmentShader("lightPos", light->getLightPos());
	shader->modifyFragmentShader("lightColor", light->getLightColur());
	shader->modifyFragmentShader("viewPos", cam.getCameraPos());

	glDrawElements(GL_TRIANGLES, mesh->getIBufferElements(), GL_UNSIGNED_INT, 0);
	mesh->unBind();
}