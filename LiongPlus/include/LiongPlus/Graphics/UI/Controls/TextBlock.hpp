// File: TextBlock.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include "_"
#include "../Control.hpp"
#include "../../../Array.hpp"
#include "../../../Media/Font.hpp"
#include "../../../Serialization/Unicode.hpp"
#include "../OpenGLBrush.hpp"

_L_NS_BEG_GRAPHICS_UI_CONTROLS

class TextBlock :
	public Control,
	public DependencyObject<TextBlock>
{
private:
	// Not a dependency property.
	Array<Rect> _GlyphBoxes;
	std::shared_ptr<TextBrush> _Brush;

public:
	const static UIProperty TextProperty;

	TextBlock() noexcept;
	~TextBlock() noexcept;

	void SetFont(const std::shared_ptr<Media::Font>& font) noexcept;

	Vector4 GetColor() const noexcept;
	void SetColor(const Vector4& color) noexcept;

	const std::u32string& GetText() const noexcept;
	void SetText(const std::u32string& unicodes) noexcept;

	/*
	 * If the text has been modified, re-render font bitmap.
	 */
	Size Layout(const Size& size) noexcept override;
	void PreRender(const ICanvas& canvas) noexcept override;
	void DoRender(const ICanvas& canvas) noexcept override;
	
	void UpdateGlyphMap() noexcept;
};

_L_NS_END_GRAPHICS_UI_CONTROLS
