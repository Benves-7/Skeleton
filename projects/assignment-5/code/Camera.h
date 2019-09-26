#pragma once
#include "config.h"
#include "exampleapp.h"
#include <cstring>
#include "MathLib.h"

class Camera
{
public:

	Camera();
	~Camera();

	Matrix4D run();
	void setup();
	/// moves the camera.
	inline void moveCameraF() { cameraPos += (cameraFront * cameraSpeed); }
	inline void moveCameraB() { cameraPos -= (cameraFront * cameraSpeed); }
	inline void moveCameraL() { cameraPos -= Vector4D::cross(cameraFront, cameraUp).Normalize() * cameraSpeed; }
	inline void moveCameraR() { cameraPos += Vector4D::cross(cameraFront, cameraUp).Normalize() * cameraSpeed; }
	inline void moveCameraU() { cameraPos += (cameraUp * cameraSpeed); }
	inline void moveCameraD() { cameraPos -= (cameraUp * cameraSpeed); }
	/// changes the camera front to rotate the camera.
	inline void changeCameraFront(Vector4D vec)
	{
		cameraFront = vec;
	}
	inline void printCameraPos()
	{
		cameraPos.Print();
	}
	inline Vector4D getCameraPos()
	{
		return cameraPos;
	}
	inline void getCamera()
	{
		printf("Camera pos = %f:%f:%f \nCamera dir = %f:%f:%f\n", cameraPos.GetX(), cameraPos.GetY(), cameraPos.GetZ(), cameraFront.GetX(), cameraFront.GetY(), cameraFront.GetZ());
	}

private:

	// Camera variables
	float radius = 2.0f;

	Vector4D cameraPos;				// Position of the camera in the world space.
	Vector4D cameraTarget;			// the point the camera looks at (used in setup).
	Vector4D cameraFront;			// A vector infront of the camera (used to make camera right and in turn camera up).
	Vector4D cameraUp;				// a vector that controlls the up direction of the camera.
	Vector4D cameraRight;			// a vector that controlls the right direction of the camera.
	float cameraSpeed = 0.5f;

	Vector4D up = Vector4D(0.0f, 1.0f, 0.0f, 1.0f); //WORLDSPACE UP

	Matrix4D view;

	Vector4D cameraDirection;

	// Perspectiv variables
	Matrix4D perspectiveProjection;
	float near = 0.1, far = 10000, top = 0.1, bot = -0.1, right = 0.1, left = -0.1;
};

inline Camera::Camera()
{
}
inline Camera::~Camera()
{
}
/// setup the camera Perspectiv.
inline void Camera::setup()
{
	perspectiveProjection = Matrix4D
	((2 * near) / (right - left), 0, 0, 0,
		0, 2 * near / (top - bot), 0, 0,
		((right + left) / (right - left)), ((top + bot) / (top - bot)), -((far + near) / (far - near)), -1,
		0, 0, -((2 * far*near) / (far - near)), 0
	);
	perspectiveProjection.TransposeThis();
	
	//Camera setup
	cameraPos = Vector4D(0.0, 2.0, 7.0, 1.0);
	cameraFront = Vector4D(0.0, 0.0, -1.0, 1.0);
	up = Vector4D(0.0f, 1.0f, 0.0f, 1.0f);
	cameraUp = up;
	cameraTarget = (cameraPos + cameraFront);


}
/// calculate the view-matrix. returns the perspecivProjection Matrix * view Matrix. 
inline Matrix4D Camera::run()
{
	cameraTarget = (cameraPos + cameraFront);
	cameraDirection = (cameraPos - cameraTarget).Normalize();
	cameraRight = Vector4D::cross(up, cameraDirection);
	cameraUp = Vector4D::cross(cameraDirection, cameraRight);

	view = Matrix4D::lookAt(cameraPos, cameraTarget, cameraUp);

	Matrix4D ret = (perspectiveProjection * view);
	ret.TransposeThis();
	return ret;

}