#pragma once
#include <string>
#include "MathLib.h"
#include <GL/glew.h>
#include <iostream>
#include <fstream>

class ShaderObject
{
public:
	bool loadShaders(const std::string& vertexShaderFile, const std::string& fragmentShaderFile);
	void modifyVertexShader(char* name_space, Matrix4D mat);
	void modifyFragmentShader(char* name_space, Vector4D vec);
	void useProgram();
	void del();

private:

	unsigned int transformLoc;
	unsigned int mProgramId;
	unsigned int GetCompiledShader(unsigned int shader_type, const std::string& shader_source);

};
/// Compile the Shaders loaded in "loadShader" takes both types of shader.
inline unsigned int ShaderObject::GetCompiledShader(unsigned int shader_type, const std::string& shader_source)
{
	unsigned int shader_id = glCreateShader(shader_type);

	const char* c_source = shader_source.c_str();
	glShaderSource(shader_id, 1, &c_source, nullptr);
	glCompileShader(shader_id);

	GLint result;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);

		GLchar* strInfoLog = new GLchar[length + 1];
		glGetShaderInfoLog(shader_id, length, &length, strInfoLog);

		fprintf(stderr, "Compilation error in shader: %s\n", strInfoLog);
		delete[] strInfoLog;
	}
	return shader_id;
}
/// Load the Shaderfiles and sends the request for GetCompiledShader,
/// also binds the program and attaches the shader to it.
inline bool ShaderObject::loadShaders(const std::string& vertexShaderFile, const std::string& fragmentShaderFile)
{
	std::ifstream is_vs(vertexShaderFile);
	const std::string f_vs((std::istreambuf_iterator<char>(is_vs)), std::istreambuf_iterator<char>());

	std::ifstream is_fs(fragmentShaderFile);
	const std::string f_fs((std::istreambuf_iterator<char>(is_fs)), std::istreambuf_iterator<char>());

	mProgramId = glCreateProgram();

	unsigned int vs = GetCompiledShader(GL_VERTEX_SHADER, f_vs);
	unsigned int fs = GetCompiledShader(GL_FRAGMENT_SHADER, f_fs);

	glAttachShader(mProgramId, vs);
	glAttachShader(mProgramId, fs);

	glLinkProgram(mProgramId);
	glValidateProgram(mProgramId);

	return true;
}
/// Puts the transform Matrix in int the shader (Camera view and everything surrounding that).
inline void ShaderObject::modifyVertexShader(char* name_space, Matrix4D mat)
{
	mat.GetArr();
	unsigned int transformLoc = glGetUniformLocation(mProgramId, name_space);
	glUniformMatrix4fv(transformLoc, 1, GL_TRUE, mat.GetPointer());
}
/// So far nothing, not used and don't know why or what it is suppose to do..
inline void ShaderObject::modifyFragmentShader(char* name_space, Vector4D vec)
{
	unsigned int transformLocation = glGetUniformLocation(mProgramId, name_space);
	glUniform3fv(transformLocation, 1, vec.GetPointer());
}
/// Sets glUseProgram to the internal program.
inline void ShaderObject::useProgram()
{
	glUseProgram(mProgramId);
}
/// Deletes the shader program.
inline void ShaderObject::del()
{
	glDeleteProgram(mProgramId);
}