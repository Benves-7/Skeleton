//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2018 Individual contributors, see AUTHORS file (Benjamin Vesterlund)
//------------------------------------------------------------------------------
#include "config.h"
#include "exampleapp.h"

#include <cstring>



const GLchar* vertexShaderSource =
"#version 430\n"
"layout(location=0) in vec3 pos;\n"
"layout(location=1) in vec4 color;\n"
"layout(location=2) in vec2 texCord;\n"
"layout(location=0) out vec4 Color;\n"
"layout(location=1) out vec2 TexCord;\n"
"uniform mat4 transform;\n"
"void main()\n"
"{\n"
"	gl_Position = transform * vec4(pos, 1.0f);\n"
"	Color = color;\n"
"   TexCord = texCord;\n"
"}\n";

const GLchar* pixelShaderSource =
"#version 430\n"
"layout(location=0) in vec4 color;\n"
"layout(location=1) in vec2 texCord;\n"
"out vec4 Color;\n"
"uniform sampler2D ourTexture;\n"
"void main()\n"
"{\n"
"	Color = texture(ourTexture, texCord);\n"
"}\n";

using namespace Display;
namespace Example
{
	float vertices[] =
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
	int indices[] =
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
		window->SetKeyPressFunction([this](int32, int32, int32, int32)
		{
			this->window->Close();
		});

		if (this->window->Open())
		{

			// Z-buffer
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);

			mesh = MeshResource(vertices, sizeof(vertices), indices, sizeof(indices));

			// set clear color to gray
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

			// setup vertex shader
			this->vertexShader = glCreateShader(GL_VERTEX_SHADER);
			GLint length = std::strlen(vertexShaderSource);
			glShaderSource(this->vertexShader, 1, &vertexShaderSource, &length);
			glCompileShader(this->vertexShader);

			// setup pixel shader
			this->pixelShader = glCreateShader(GL_FRAGMENT_SHADER);
			length = std::strlen(pixelShaderSource);
			glShaderSource(this->pixelShader, 1, &pixelShaderSource, &length);
			glCompileShader(this->pixelShader);

			// create a program object
			this->program = glCreateProgram();
			glAttachShader(this->program, this->vertexShader);
			glAttachShader(this->program, this->pixelShader);
			glLinkProgram(this->program);

			// setup vertex buffer object VBO
			mesh.setup();					//VAO
			mesh.bindVertexbuffer();		//VBO
			mesh.bindIndexBuffer();			//EBO
			mesh.bindPointers();			//read protocols
			textureResource.makeTexture();

			perspectiveProjection = Matrix4D(
				2 * n / (r - l), 0, 0, 0,
				0, 2 * n / (t - b), 0, 0,
				((r + l) / (r - l)), ((t + b) / (t - b)), -((f + n) / (f - n)), -1,
				0, 0, -((2 * f*n) / (f - n)), 0
			);
			perspectiveProjection.TransposeThis();

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
		float move = 0;
		while (this->window->IsOpen())
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			this->window->Update();

			glUseProgram(this->program);
			// do stuff

			mesh.bind(); //VAO

			// bunch of code to handel my bad mathlib..
			Matrix4D rotz = Matrix4D::RotZ(rot);
			Matrix4D roty = Matrix4D::RotY(rot);
			Matrix4D rotx = Matrix4D::RotX(rot);
			Matrix4D position = Matrix4D::GetPositionMatrix(Vector4D(sin(move), 0.0f, 0.0f, 0.0f));
			Matrix4D transform = roty * rotx;
			transform.GetArr();

			//camera work
			float radius = 2.0f;
			float camX = sin(glfwGetTime()) * radius;
			float camZ = cos(glfwGetTime()) * radius;
			Vector4D cameraPos(camX, 0.0f, camZ, 1.0f);
			Vector4D cameraTarget(0.0f, 0.0f, 0.0f, 1.0f);
			Vector4D up(0.0f, 1.0f, 0.0f, 1.0f);
			Vector4D cameraDirection = (cameraPos - cameraTarget).Normalize();
			Vector4D cameraRight = Vector4D::cross(up, cameraDirection).Normalize();
			Vector4D cameraUp = Vector4D::cross(cameraDirection, cameraRight);

			//lookAt matrix
			Matrix4D view;
			view = Matrix4D::lookAt(cameraRight, up, cameraDirection, cameraPos);

			// Combining diffrent matrixes.
			Matrix4D mat = perspectiveProjection * view * rotx;
			mat.GetArr();


			unsigned int transformLoc = glGetUniformLocation(this->program, "transform");

			
			glUniformMatrix4fv(transformLoc, 1, GL_TRUE, mat.GetPointer());
			textureResource.bindTexture();
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			mesh.unBind();	//VAO

			this->window->SwapBuffers();
			//movementspeed and rotatespeed controll
			rot = rot + 0.01;
			move = move + 0.01;
		}
	}

} // namespace Example
