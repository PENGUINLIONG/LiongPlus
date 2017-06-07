// File: DependencyInjection.hpp
// Author: Rendong Liang (Liong)

#include "DependencyInjection.hpp"

_L_NS_BEG_GRAPHICS_UI

namespace details
{
	prop_set& getPropMap()
	{
		static std::unique_ptr<prop_set> _DepPropertySet;

		if (_DepPropertySet == nullptr)
			_DepPropertySet = std::make_unique<prop_set>();

		return *_DepPropertySet;
	}
	prop_set& getAttachedPropMap()
	{
		static std::unique_ptr<prop_set> _AttachedDepPropertySet;

		if (_AttachedDepPropertySet == nullptr)
			_AttachedDepPropertySet = std::make_unique<prop_set>();

		return *_AttachedDepPropertySet;
	}
}

_L_NS_END_GRAPHICS_UI

