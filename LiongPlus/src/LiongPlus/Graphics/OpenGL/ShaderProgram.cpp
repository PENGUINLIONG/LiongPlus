// File: Shader.hpp
// Author: Rendong Liang (Liong)
#include <iostream>
#include <vector>
#include "LiongPlus/Graphics/OpenGL/ShaderProgram.hpp"
#include "LiongPlus/Platform/MinOpenGL.hpp"

_L_NS_BEG_GRAPHICS_OPENGL

using namespace Platform::OpenGL;

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
	if (_Handle)
		glDeleteProgram(_Handle);
	_Handle = std::exchange(other._Handle, _Handle);
	return *this;
}

unsigned ShaderProgram::GetHandle() const noexcept
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
		glGetProgramInfoLog(_Handle, length, nullptr, msg.data());
		std::cout << msg.data() << std::endl;
	}
	_Initialize();
}

unsigned ShaderProgram::GetAttributeLocation(const std::string& name) noexcept
{
	return glGetAttribLocation(_Handle, name.c_str());
}
VertexArray ShaderProgram::InitVertexArray() noexcept
{
	VertexArray arr;
	int count;
	glGetProgramiv(_Handle, GL_ACTIVE_ATTRIBUTES, &count);
	arr.EnableAttributes(count);
	return arr;
}

unsigned ShaderProgram::GetUniformLocation(const std::string& name) noexcept
{
	return glGetUniformLocation(_Handle, name.c_str());
}
void ShaderProgram::SetSampler(const std::string& name, int sampler) noexcept
{
	SetSampler(GetUniformLocation(name.c_str()), sampler);
}
void ShaderProgram::SetSampler(unsigned location, int sampler) noexcept
{
	if (glProgramUniform1i)
		glProgramUniform1i(_Handle, location, sampler);
	else
	{
		auto act = Activate();
		glUniform1i(location, sampler);
	}
}

ShaderProgram::ActivationGuard ShaderProgram::Activate() const
{
	return { _Handle };
}

UniformBuffer ShaderProgram::InitUniformBlock(const std::string& name,
	void* init) noexcept
{
	UniformBuffer buf;
	auto index = GetUniformBlockIndex(name);
	// Allocate memory and bind.
	buf.Allocate(GetUniformBlockSize(index), true);
	if (init) buf.Write(init);
	glUniformBlockBinding(_Handle, index, index);

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

ShaderProgram::ActivationGuard::ActivationGuard(unsigned handle) :
	_Flag(true)
{
	glUseProgram(handle);
}
ShaderProgram::ActivationGuard::ActivationGuard(ActivationGuard&& inst) :
	_Flag(std::exchange(inst._Flag, false))
{
}
ShaderProgram::ActivationGuard::~ActivationGuard()
{
	glUseProgram(0);
}

_L_NS_END_GRAPHICS_OPENGL
