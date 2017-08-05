// File: Sampler.hpp
// Author: Rendong Liang (Liong)
#include <utility>
#include "LiongPlus/Graphics/OpenGL/Sampler.hpp"
#include "LiongPlus/Platform/MinOpenGL.hpp"

_L_NS_BEG_GRAPHICS_OPENGL

using namespace Platform::OpenGL;

Sampler::Sampler()
	: _Handle()
{
	if (glCreateSamplers)
		glCreateSamplers(1, &_Handle);
	else
		glGenSamplers(1, &_Handle);
}
Sampler::Sampler(Sampler&& inst)
	: _Handle(std::exchange(inst._Handle, 0))
{
}
Sampler::~Sampler()
{
	_Clear();
}

void Sampler::_Clear()
{
	if (_Handle)
		glDeleteSamplers(1, &_Handle);
}

Sampler& Sampler::operator=(Sampler&& inst)
{
	_Clear();
	_Handle = std::exchange(inst._Handle, 0);
	return *this;
}

Sampler::ActivationGuard Sampler::Activate(unsigned unit) const
{
	return { unit, _Handle };
}

Sampler::ActivationGuard::ActivationGuard(unsigned unit, unsigned handle) :
	_Unit(unit)
{
	glBindSampler(unit, handle);
}
Sampler::ActivationGuard::ActivationGuard(ActivationGuard&& inst) :
	_Unit(std::exchange(inst._Unit, 0))
{
}
Sampler::ActivationGuard::~ActivationGuard()
{
	if (_Unit)
		glBindSampler(_Unit, 0);
}

_L_NS_END_GRAPHICS_OPENGL
