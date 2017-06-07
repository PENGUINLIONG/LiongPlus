// File: Shader.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include <string>
#include "_"

_L_NS_BEG_GRAPHICS_OPENGL

enum class ShaderType {
	Fragment = GL_FRAGMENT_SHADER,
	Geometry = GL_GEOMETRY_SHADER,
	TesselationControl = GL_TESS_CONTROL_SHADER,
	TesselationEvaluation = GL_TESS_EVALUATION_SHADER,
	Vertex = GL_VERTEX_SHADER,
};

class Shader
{
	friend class ShaderProgram;
private:
	ShaderType _Type;
	GLuint _Handle;

public:
	Shader(ShaderType type) noexcept;
	Shader(const Shader&) = delete;
	Shader(Shader&&) noexcept;
	~Shader();

	Shader& operator=(const Shader&) = delete;
	Shader& operator=(Shader&&) noexcept;

	GLuint GetHandle() const noexcept;
	ShaderType GetShaderType() const noexcept;

	bool Compile(const std::string& src) noexcept;
};

_L_NS_END_GRAPHICS_OPENGL
