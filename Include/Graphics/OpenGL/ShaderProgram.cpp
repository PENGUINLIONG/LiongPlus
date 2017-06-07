// File: Shader.hpp
// Author: Rendong Liang (Liong)
#include <iostream>
#include <vector>
#include "ShaderProgram.hpp"

_L_NS_BEG_GRAPHICS_OPENGL

namespace details
{
	using namespace std;

	inline auto calcResourceNameLength(GLuint prog, GLenum interf)
	{
		GLint len;
		glGetProgramInterfaceiv(prog, interf, GL_MAX_NAME_LENGTH, &len);
		return len;
	}

	inline auto getResourceCount(GLuint prog, GLenum interf)
	{
		GLint count;
		glGetProgramInterfaceiv(prog, interf, GL_ACTIVE_RESOURCES, &count);
		return count;
	}
}

ShaderProgram::ShaderProgram() noexcept :
	_Handle(glCreateProgram())
{
}
ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept :
	_Handle()
{
	*this = std::forward<ShaderProgram>(other);
}
ShaderProgram::~ShaderProgram() noexcept
{
	if (_Handle)
		glDeleteProgram(_Handle);
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept
{
	_Handle = std::exchange(other._Handle, _Handle);
	return *this;
}

GLuint ShaderProgram::GetHandle() const noexcept
{
	return _Handle;
}

void ShaderProgram::_Initialize() noexcept
{
	using namespace details;

	GLint actualLength = 0, location = 0, count = 0;
	vector<char> nameCache;
	
	// Inject textures.
	nameCache.resize(calcResourceNameLength(_Handle, GL_UNIFORM) + 1);
	count = getResourceCount(_Handle, GL_UNIFORM);
	for (unsigned i = 0; i < count; ++i)
	{
		glGetActiveUniformName(_Handle, i, nameCache.size(),
			&actualLength, nameCache.data());
		GLint loc = glGetUniformLocation(_Handle, nameCache.data());
		// Ignore all uniform fields in uniform blocks.
		if (loc < 0) continue;
		glProgramUniform1i(_Handle, loc, i);
	}
}

void ShaderProgram::Attach(const Shader& shader) noexcept
{
	glAttachShader(_Handle, shader.GetHandle());
}
void ShaderProgram::Detach(const Shader& shader) noexcept
{
	glDetachShader(_Handle, shader.GetHandle());
}
void ShaderProgram::Link() noexcept
{
	glLinkProgram(_Handle);

	GLint result = GL_FALSE;
	GLint length = 0;

	glGetProgramiv(_Handle, GL_LINK_STATUS, &result);
	glGetProgramiv(_Handle, GL_INFO_LOG_LENGTH, &length);
	if (!result)
	{
		std::vector<char> msg(length + 1);
		glGetProgramInfoLog(_Handle, length, NULL, msg.data());
		std::cout << msg.data() << std::endl;
	}
	_Initialize();
}

GLuint ShaderProgram::GetAttributeLocation(const std::string& name) noexcept
{
	return glGetAttribLocation(_Handle, name.c_str());
}

GLuint ShaderProgram::GetUniformLocation(const std::string& name) noexcept
{
	return glGetUniformLocation(_Handle, name.c_str());
}

GLuint ShaderProgram::InitUniformBlock(const std::string& name,
	unsigned bindingPoint) noexcept
{
	GLuint buf;
	glGenBuffers(1, &buf);
	auto index = GetUniformBlockIndex(name);
	// Allocate memory and bind.
	glUniformBlockBinding(_Handle, index, bindingPoint);
	glBindBuffer(GL_UNIFORM_BUFFER, buf);
	glBufferData(GL_UNIFORM_BUFFER, GetUniformBlockSize(index), nullptr, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	return buf;
}
unsigned ShaderProgram::GetUniformBlockIndex(
	const std::string& name) noexcept
{
	return glGetUniformBlockIndex(_Handle, name.c_str());
}
size_t ShaderProgram::GetUniformBlockSize(unsigned index) noexcept
{
	GLint solidSettingsSize;
	glGetActiveUniformBlockiv(_Handle, index, GL_UNIFORM_BLOCK_DATA_SIZE,
		&solidSettingsSize);
	return solidSettingsSize;
}

void ShaderProgram::Shade(GLenum mode, size_t to, size_t from) const noexcept
{
	glUseProgram(_Handle);
	glDrawArrays(mode, from, to);
	glUseProgram(0);
}

_L_NS_END_GRAPHICS_OPENGL
