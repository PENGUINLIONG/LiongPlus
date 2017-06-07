// File: Doodle.hpp
// Author: Rendong Liang (Liong)
#include "_"
#include "../Control.hpp"

/*
 * Doodle exposes rendering procedure to user. There is no constraints
 *   about how rendering should be done, i.e., The user can draw through
 *   LiongPlus Canvas or OpenGL, DirectX native API (in case a
 *   suitable render context was created.)
 */
_L_NS_BEG_GRAPHICS_UI_CONTROLS

struct RenderEventArgs : public EventArgs
{
public:
	ICanvas Canvas;
};

class Doodle
	: public Control
{
public:
	UIEvent<RenderEventArgs> Render;

	void DoRender(const ICanvas& canvas) override;
};


_L_NS_END_GRAPHICS_UI_CONTROLS
