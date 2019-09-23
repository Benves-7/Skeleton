//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2018 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "exampleapp.h"
#include "MathLib.h"
#include "hitable_list.h"
#include "sphere.h"
#include "lambertian.h"
#include "camera.h"
#include "metal.h"
#include "dielectric.h"
#include <cstring>
#include <vector>
#include <chrono>
#include "GLFW/glfw3.h"

const GLchar* vertexShaderSource =
"#version 430\n"
"layout(location=0) in vec3 pos;\n"
"layout(location=1) in vec4 color;\n"
"layout(location=0) out vec4 Color;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(pos, 1);\n"
"	Color = color;\n"
"}\n";

const GLchar* pixelShaderSource =
"#version 430\n"
"layout(location=0) in vec4 color;\n"
"out vec4 Color;\n"
"void main()\n"
"{\n"
"	Color = color;\n"
"}\n";


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

    int nx = 1000, ny = 500;
    const int numSpheres = 25;         // number of spheres in the world.
    // Framebuffer setup
    GLuint frameCopy;
    GLuint texture;

    hitable* world;
    hitable* list[numSpheres];

    // create camera.
    Vector4D lookfrom(0,5,3), lookat(0,0,-1);
    float dist_to_focus = ((lookfrom-lookat).Length()), aperture = 0.15;
    camera* cam = new camera(lookfrom, lookat, Vector4D(0,1,0), 90, float(nx)/float(ny), aperture, dist_to_focus);
    int size = ny*nx; // ny = 768, nx = 1024
    std::vector<float> frameBuffer;
    std::vector<float> accumulationBuffer;

bool
ExampleApp::Open()
{

    frameBuffer.resize(size * 3);
    accumulationBuffer.resize(size * 3);
	App::Open();
	this->window = new Display::Window;

	window->SetSize(nx, ny);
	//window->GetSize(nx, ny);

    const float maxPos = 10.0f;
    auto randCoord = [maxPos]() {
        return (1 - (drand48() * 2)) * maxPos;
    };
    const float maxSize = 3.0f;
    const float minSize = 0.1f;
    auto randSize = [maxSize, minSize]() {
        return minSize + (drand48() * maxSize);
    };

    // create world to render.
    list[0] = new sphere(Vector4D(0,-5000,0), 5000, new lambertian(Vector4D(0.5,0.5,0.5)));
    list[1] = new sphere(Vector4D(0,1,-1), 1, new metal(Vector4D(1.0, 1.0,1.0), 0.1));
    for (int a = 2; a < numSpheres; a++)
    {
        float choose_mat = drand48();
        float size = randSize();
        Vector4D center(randCoord(), size/2.0, randCoord());
        if (choose_mat < 0.7)           // diffuse
        {
            list[a] = new sphere(center, size/2.0, new lambertian(Vector4D(drand48(),drand48(),drand48())));
        }
        else if (choose_mat < 0.9)     // metal
        {
            list[a] = new sphere(center, size/2.0, new metal(Vector4D(drand48(),drand48(),drand48()), drand48()));
        }
        else                            // glass
        {
            list[a] = new sphere(center, size/2.0, new dielectric(1.5));
        }
    }
    world = new hitable_list(list, numSpheres);



    if (this->window->Open())
	{
        /*window->SetMouseMoveFunction([this](float64 xpos, float64 ypos)
        {
            float radianconversion = 3.1415926 / 180; // g�nger f�r att f� radian.

            if (this->leftmousebuttonpressed && lastX != 0)
            {
                float xoffset = xpos - lastX;
                float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
                lastX = xpos;
                lastY = ypos;

                float sensitivity = 0.15f;
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
                cam->changeCameraFront(front);
            }
            lastX = xpos; lastY = ypos;
        });*/

        window->SetKeyPressFunction([this](int32 key, int32 i, int32 action, int32 modifier)
        {
            if ((key == GLFW_KEY_A || key == GLFW_KEY_LEFT) && (action == GLFW_REPEAT || action == GLFW_PRESS))
            {
                cam->moveCameraL();
            }
            if ((key == GLFW_KEY_D || key == GLFW_KEY_RIGHT) && (action == GLFW_REPEAT || action == GLFW_PRESS))
            {
                cam->moveCameraR();
            }
            if ((key == GLFW_KEY_W || key == GLFW_KEY_UP) && (action == GLFW_REPEAT || action == GLFW_PRESS))
            {
                cam->moveCameraF();
            }
            if ((key == GLFW_KEY_S || key == GLFW_KEY_DOWN) && (action == GLFW_REPEAT || action == GLFW_PRESS))
            {
                cam->moveCameraB();
            }
            if (key == GLFW_KEY_SPACE && (action == GLFW_REPEAT || action == GLFW_PRESS))
            {
                cam->moveCameraU();
            }
            if (key == GLFW_KEY_Q && (action == GLFW_REPEAT || action == GLFW_PRESS))
            {
                cam->moveCameraD();
            }

            if (key == GLFW_KEY_ESCAPE)
                this->window->Close();
        });

        glGenFramebuffers(1, &frameCopy);
        glBindFramebuffer(GL_FRAMEBUFFER, frameCopy);

        glGenTextures(1, &texture);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, nx, ny, 0, GL_RGB, GL_FLOAT, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);

		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
/**
*/

Vector4D color(const ray& r, hitable* world, int depth) {
        hit_record rec;
        if (world->hit(r, 0.001, MAXFLOAT, rec))
        {
            ray scattered;
            Vector4D attenuation;
            // number of bounces.
            if (depth < 5 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
                return attenuation * color(scattered, world, depth + 1);
            } else
                return Vector4D(0, 0, 0);
        }
        else
        {
            Vector4D unit_direction = r.direction().Normalize();
            float t = (unit_direction.y() + 1.0) * 0.5;
            return Vector4D(1.0, 1.0, 1.0) * (1.0 - t) + Vector4D(0.5, 0.7, 1.0) * t;
        }
    }
void
ExampleApp::Run()
{
    uint loops = 0;
    while (this->window->IsOpen())
    {
        auto start = std::chrono::system_clock::now();
        loops++;
        int index = 0;
        for (int j = 0; j < ny; ++j)
        {
            for (int i = 0; i < nx; ++i)
            {
                ray r = cam->get_ray(float(i + drand48()) / float(nx), float(j + drand48()) / float(ny));
                Vector4D p = r.point_at_parameter(2.0);
                Vector4D col(accumulationBuffer[index], accumulationBuffer[index + 1], accumulationBuffer[index + 2]);
                col.addTo(color(r, world,0));

                accumulationBuffer[index] = col.r();
                accumulationBuffer[index + 1] = col.g();
                accumulationBuffer[index + 2] = col.b();

                frameBuffer[index++] = accumulationBuffer[index] / loops;
                frameBuffer[index++] = accumulationBuffer[index] / loops;
                frameBuffer[index++] = accumulationBuffer[index] / loops;
            }
        }
        auto end = std::chrono::system_clock::now();

        std::chrono::duration<double> elapsed_seconds = end-start;
        std::time_t end_time = std::chrono::system_clock::to_time_t(end);

        std::cout << "finished computation at " << std::ctime(&end_time)
                  << "elapsed time: " << elapsed_seconds.count() << "s\n"
                  << "number of loops " << loops << std::endl;

        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, nx, ny, 0, GL_RGB, GL_FLOAT, (void*)&frameBuffer[0]);
        glBindTexture(GL_TEXTURE_2D, 0);
        int32 width, height;
        this->window->GetSize(width, height);
        glBlitNamedFramebuffer(frameCopy, NULL, 0, 0, nx, ny, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        this->window->SwapBuffers();
	}
}

} // namespace Example