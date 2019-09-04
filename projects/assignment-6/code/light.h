#pragma once

#include "MathLib.h"

class LightNode
{
public:
	inline LightNode() { lightPos = Vector4D(0.0, 2.0, 7.0, 1.0); lightColur = Vector4D(0.5f, 0.5f, 0.5f, 1.0); }
	inline ~LightNode() {  }
	inline void setLightPos(Vector4D vec) { lightPos = vec; }
	inline void setLightColur(Vector4D vec) { lightColur = vec; }
	inline Vector4D getLightPos() { return lightPos; }
	inline Vector4D getLightColur() { return lightColur; }

private:
	static int id;
	Vector4D lightPos;
	Vector4D lightColur;
	float intensity;

};
