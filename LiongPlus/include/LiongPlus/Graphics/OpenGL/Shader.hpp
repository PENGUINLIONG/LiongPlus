// File: Shader.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include <string>
#include "_"

_L_NS_BEG_GRAPHICS_OPENGL

enum class ShaderType
{
	Fragment = 0x8B30,
	Geometry = 0x8DD9,
	TesselationControl = 0x8E88,
	TesselationEvaluation = 0x8E87,
	Vertex = 0x8B31,
};

class Shader
{
	friend class ShaderProgram;
private:
	unsigned _Type;
	unsigned _Handle;

public:
	Shader(ShaderType type) noexcept;
	Shader(const Shader&) = delete;
	Shader(Shader&&) noexcept;
	~Shader();

	Shader& operator=(const Shader&) = delete;
	Shader& operator=(Shader&&) noexcept;

	unsigned GetHandle() const noexcept;
	ShaderType GetShaderType() const noexcept;

	bool Compile(const std::string& src) noexcept;
};

_L_NS_END_GRAPHICS_OPENGL
