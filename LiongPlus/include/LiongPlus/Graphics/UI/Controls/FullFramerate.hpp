// File: FullFramerate.hpp
// Author: Rendong Liang (Liong)
#include "_"
#include "../Control.hpp"
#include "../UIFactory.hpp"

// This object has no size

_L_NS_BEG_GRAPHICS_UI_CONTROLS

class FullFramerate :
	public UIElement,
	public DependencyObject<FullFramerate>
{
private:
	static const UIProperty RenderTriggerProperty;

public:
	Size Layout(const Size& containerSize) override { return { 0, 0 }; }
	void PreRender(const ICanvas& canvas) override {}
	void DoRender(const ICanvas& canvas) override {}
	void PostRender(const ICanvas& canvas) override;
};

_L_NS_END_GRAPHICS_UI_CONTROLS

_L_NS_BEG_GRAPHICS_UI

template<>
struct UIFactory<Controls::FullFramerate>
{
public:
	std::shared_ptr<Controls::FullFramerate> Create();
};

_L_NS_END_GRAPHICS_UI
