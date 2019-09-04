#pragma once
//------------------------------------------------------------------------------
/**
	Application class used for example application.
	
	(C) 2015-2018 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/app.h"
#include "render/window.h"
#include "GraphicsNode.h"

#include "MathLib.h"
#include "meshresource.h"
#include "textureresource.h"
#include "ShaderObject.h"

namespace Example
{
	class ExampleApp : public Core::App
	{
	public:

		/// constructor
		ExampleApp();
		/// destructor
		~ExampleApp();

		void setNodeClass(std::shared_ptr<GraphicsNode> node);
		GraphicsNode* getNodeClass();

		/// open app
		bool Open();
		/// run app
		void Run();

	private:

		float lastX = 0, lastY = 0;
		float yaw = -90.0f, pitch = 0.0f;
		bool leftmousebuttonpressed = false;
		Display::Window* window;
		vector<GraphicsNode> nodes;
		Camera camera_;
		vector<shared_ptr<LightNode>> lights;

	};
}