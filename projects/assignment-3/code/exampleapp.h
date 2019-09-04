#pragma once
//------------------------------------------------------------------------------
/**
	Application class used for example application.
	
	(C) 2015-2018 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/app.h"
#include "render/window.h"
#include "meshresource.h"
#include "MathLib.h"
#include "textureresource.h"

namespace Example
{
class ExampleApp : public Core::App
{
public:

	/// constructor
	ExampleApp();
	/// destructor
	~ExampleApp();

	/// open app
	bool Open();
	/// run app
	void Run();

private:

	GLuint program;
	GLuint vertexShader;
	GLuint pixelShader;
	GLuint triangle;
	Display::Window* window;
	MeshResource mesh;
	TextureResource textureResource;
	Matrix4D perspectiveProjection;

	float n = 0.1, f = 10, t = 0.1, b = -0.1, r = 0.1, l = -0.1;

};
} // namespace Example