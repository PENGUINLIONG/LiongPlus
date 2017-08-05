// File: InputField.hpp
// Author: Rendong Liang (Liong)
#include "_"
#include "../Control.hpp"
#include "TextBlock.hpp"

_L_NS_BEG_GRAPHICS_UI_CONTROLS

class InputField
	: public Control
{
protected:
	unsigned CursorPosition;
	TextBlock _Text();

public:
	void OnKeyDown(KeyboardEventArgs& args) override;
	void OnCharInput(CharInputEventArgs& args) override;

	Size Layout(const Size& containerSize) override;
	void DoRender(const ICanvas& canvas) override;
};

_L_NS_END_GRAPHICS_UI_CONTROLS
