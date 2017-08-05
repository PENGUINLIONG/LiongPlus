// File: UIUpdater.hpp
// Author: Rendong Liang (Liong)
#include "_"

_L_NS_BEG_GRAPHICS_UI

namespace UIUpdater
{
	void StartUIUpdater();
	void StopUIUpdater();
}

struct UIUpdaterGuard
{
public:
	UIUpdaterGuard();
	~UIUpdaterGuard();
};

_L_NS_END_GRAPHICS_UI
