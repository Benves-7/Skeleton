//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2018 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "exampleapp.h"

#include <cstring>

using namespace Display;
namespace Example
{
	//------------------------------------------------------------------------------
	/**
	*/
	ExampleApp::ExampleApp()
	{
		// empty
	}

	//------------------------------------------------------------------------------
	/**
	*/
	ExampleApp::~ExampleApp()
	{
		// empty
	}

	//------------------------------------------------------------------------------
	/**
	*/
	bool
		ExampleApp::Open()
	{
		App::Open();
		this->window = new Display::Window;

		if (this->window->Open())
		{


			// Z-buffer
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);

			// set clear color to gray
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

			//Setup
			std::shared_ptr<GraphicsNode> tempNode = std::make_shared<GraphicsNode>();
			this->setNodeClass(tempNode);
			std::shared_ptr<MeshResource> mesh = std::make_shared<MeshResource>();
			std::shared_ptr<TextureResource> texture = std::make_shared<TextureResource>();
			std::shared_ptr<ShaderObject> shader = std::make_shared<ShaderObject>();

			node->setMeshClass(mesh);
			node->setTextureResource(texture);
			node->setShaderObject(shader);
			node->setup("Shaders/vs.shader", "Shaders/fs.shader", "Texture/container.jpg");
			node->getMeshClass()->translate(Vector4D(0.0, 0.0, 1.0, 1.0));
			

			window->SetMousePressFunction([this](int32 button, int32 action, int32 mods)
			{
				if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
				{
					printf("pressed\n");
					this->leftmousebuttonpressed = true;
					lastX = 0;
				}
				else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
				{
					printf("released\n");
					this->leftmousebuttonpressed = false;
				}
			});

			window->SetMouseMoveFunction([this](float64 xpos, float64 ypos)
			{
				float radianconversion = 3.1415926 / 180; // gånger för att få radian.

				if (this->leftmousebuttonpressed && lastX != 0)
				{

					printf("moving\n");
					
					float xoffset = xpos - lastX;
					float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
					lastX = xpos;
					lastY = ypos;

					float sensitivity = 0.05f;
					xoffset *= sensitivity;
					yoffset *= sensitivity;

					yaw += xoffset;
					pitch += yoffset;

					if (pitch > 89.0f)
						pitch = 89.0f;
					if (pitch < -89.0f)
						pitch = -89.0f;

					Vector4D front;
					front.SetX(cos(pitch * radianconversion) * cos(yaw * radianconversion));
					front.SetY(sin(pitch * radianconversion));
					front.SetZ(cos(pitch * radianconversion) * sin(yaw * radianconversion));
					Vector4D temp = front.Normalize();
					node->camera.changeCameraFront(front.Normalize());
				}
				lastX = xpos; lastY = ypos;
			});

			window->SetKeyPressFunction([this](int32 key, int32 i, int32 action, int32 modifier)
			{
				if ((key == GLFW_KEY_A || key == GLFW_KEY_LEFT) && (action == GLFW_REPEAT || action == GLFW_PRESS))
				{
					printf("left\n");
					node->camera.moveCameraL();
				}
				if ((key == GLFW_KEY_D || key == GLFW_KEY_RIGHT) && (action == GLFW_REPEAT || action == GLFW_PRESS))
				{
					printf("right\n");
					node->camera.moveCameraR();
				}
				if ((key == GLFW_KEY_W || key == GLFW_KEY_UP) && (action == GLFW_REPEAT || action == GLFW_PRESS))
				{
					printf("forward\n");
					node->camera.moveCameraF();
				}
				if ((key == GLFW_KEY_S || key == GLFW_KEY_DOWN) && (action == GLFW_REPEAT || action == GLFW_PRESS))
				{
					printf("back\n");
					node->camera.moveCameraB();
				}
				if (key == GLFW_KEY_ESCAPE)
					this->window->Close();
			});

			return true;
		}
		return false;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
		ExampleApp::Run()
	{
		float rot = 0;
		while (this->window->IsOpen())
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			this->window->Update();

			node->draw();

			this->window->SwapBuffers();
			rot = rot + 0.01;
		}
	}

} // namespace Example