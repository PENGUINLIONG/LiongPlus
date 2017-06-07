// File: Shader.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include "_"
#include "Shader.hpp"

_L_NS_BEG_GRAPHICS_OPENGL

/*
 * This implementation of shader program does NOT support uniform
 *   variable insertion in default block. Use std140 Uniform Block
 *   Objects to pass non-handle variables instead.
 */
class ShaderProgram
{
private:
	GLuint _Handle;
	void _Initialize() noexcept;

public:
	ShaderProgram() noexcept;
	ShaderProgram(const ShaderProgram&) = delete;
	ShaderProgram(ShaderProgram&&) noexcept;
	~ShaderProgram() noexcept;

	ShaderProgram& operator=(const ShaderProgram&) = delete;
	ShaderProgram& operator=(ShaderProgram&&) noexcept;

	GLuint GetHandle() const noexcept;

	void Attach(const Shader& shader) noexcept;
	void Detach(const Shader& shader) noexcept;
	void Link() noexcept;

	GLuint GetAttributeLocation(const std::string& name) noexcept;

	GLuint GetUniformLocation(const std::string& name) noexcept;

	GLuint InitUniformBlock(const std::string& name,
		unsigned bindingPoint) noexcept;
	unsigned GetUniformBlockIndex(const std::string& name) noexcept;
	size_t GetUniformBlockSize(unsigned index) noexcept;

	void Shade(GLenum mode, size_t to, size_t from = 0)
		const noexcept;
};

struct UseShaderProgram
{
	UseShaderProgram(GLuint prog) noexcept
	{
		glUseProgram(prog);
	}
	~UseShaderProgram() noexcept
	{
		glUseProgram(0);
	}
};

_L_NS_END_GRAPHICS_OPENGL
