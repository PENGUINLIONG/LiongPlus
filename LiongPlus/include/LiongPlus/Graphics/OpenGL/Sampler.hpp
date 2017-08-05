// File: Sampler.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include "_"

_L_NS_BEG_GRAPHICS_OPENGL

class Sampler
{
private:
	unsigned _Handle;

	void _Clear();

public:
	class ActivationGuard
	{
	private:
		unsigned _Unit;

	public:
		ActivationGuard(unsigned unit, unsigned handle);
		ActivationGuard(ActivationGuard&&);
		~ActivationGuard();
	};

	Sampler();
	Sampler(Sampler&&);
	~Sampler();

	Sampler& operator=(Sampler&&);

	ActivationGuard Activate(unsigned unit) const;
};

_L_NS_END_GRAPHICS_OPENGL
