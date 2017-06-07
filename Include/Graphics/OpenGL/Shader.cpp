// File: Shader.cpp
// Author: Rendong Liang (Liong)
#include <iostream>
#include <vector>
#include "Shader.hpp"

_L_NS_BEG_GRAPHICS_OPENGL

using namespace std;

//
// Shader
//

Shader::Shader(ShaderType type) noexcept :
	_Type(type),
	_Handle(glCreateShader((GLenum)type))
{
}
Shader::Shader(Shader&& inst) noexcept :
	_Type(),
	_Handle()
{
	*this = std::forward<Shader>(inst);
}
Shader::~Shader() noexcept
{
	if (_Handle)
		glDeleteShader(_Handle);
}

Shader& Shader::operator=(Shader&& other) noexcept
{
	_Type = std::exchange(other._Type, _Type);
	_Handle = std::exchange(other._Handle, _Handle);
	return *this;
}

GLuint Shader::GetHandle() const noexcept
{
	return _Handle;
}
ShaderType Shader::GetShaderType() const noexcept
{
	return _Type;
}

bool Shader::Compile(const std::string& src) noexcept
{
	auto srcPtr = src.c_str();
	glShaderSource(_Handle, 1, &srcPtr, nullptr);
	glCompileShader(_Handle);

	GLint isCompiled = 0;
	glGetShaderiv(_Handle, GL_COMPILE_STATUS, &isCompiled);
	if (!isCompiled)
	{
		GLint maxLength = 0;
		glGetShaderiv(_Handle, GL_INFO_LOG_LENGTH, &maxLength);

		// $maxLength includes the NULL character.
		std::vector<GLchar> errorLog(maxLength + 1);
		glGetShaderInfoLog(_Handle, maxLength, &maxLength, errorLog.data());
		std::cout << errorLog.data() << std::endl;

		glDeleteShader(_Handle);
	}
	glReleaseShaderCompiler();
	return isCompiled;
}

_L_NS_END_GRAPHICS_OPENGL
