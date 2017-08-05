// File: Shader.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include "_"
#include "BufferObject.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"

_L_NS_BEG_GRAPHICS_OPENGL

/*
 * This implementation of shader program does NOT support uniform
 *   variable insertion in default block. Use std140 Uniform Block
 *   Objects to pass non-handle variables instead.
 */
class ShaderProgram
{
private:
	unsigned _Handle;
	void _Initialize() noexcept;

public:
	ShaderProgram() noexcept;
	ShaderProgram(const ShaderProgram&) = delete;
	ShaderProgram(ShaderProgram&&) noexcept;
	~ShaderProgram() noexcept;

	ShaderProgram& operator=(const ShaderProgram&) = delete;
	ShaderProgram& operator=(ShaderProgram&&) noexcept;

	struct ActivationGuard
	{
	private:
		bool _Flag;

	public:
		ActivationGuard(unsigned handle);
		ActivationGuard(ActivationGuard&&);
		~ActivationGuard();
	};

	unsigned GetHandle() const noexcept;

	void Attach(const Shader& shader) noexcept;
	void Detach(const Shader& shader) noexcept;
	void Link() noexcept;

	unsigned GetAttributeLocation(const std::string& name) noexcept;
	VertexArray InitVertexArray() noexcept;

	unsigned GetUniformLocation(const std::string& name) noexcept;
	void SetSampler(const std::string& name, int sampler) noexcept;
	void SetSampler(unsigned location, int sampler) noexcept;

	UniformBuffer InitUniformBlock(const std::string& name,
		void* init) noexcept;
	unsigned GetUniformBlockIndex(const std::string& name) noexcept;
	size_t GetUniformBlockSize(unsigned index) noexcept;

	ActivationGuard Activate() const;
};

_L_NS_END_GRAPHICS_OPENGL
