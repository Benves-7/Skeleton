#pragma once
//------------------------------------------------------------------------------
/**
	Application class used for example application.
	
	(C) 2015-2018 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/app.h"
#include "render/window.h"
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

	Display::Window* window;
private:

    float lastX = 0, lastY = 0;
    float yaw = -90.0f, pitch = 0.0f;
    bool leftmousebuttonpressed = false;
	GLuint program;
	GLuint vertexShader;
	GLuint pixelShader;
	GLuint triangle;
};
} // namespace Example