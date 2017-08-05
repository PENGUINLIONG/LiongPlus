// File: TextBlock.cpp
// Author: Rendong Liang (Liong)
#include "LiongPlus/Graphics/UI/Controls/TextBlock.hpp"

_L_NS_BEG_GRAPHICS_UI_CONTROLS

const UIProperty TextBlock::TextProperty =
UIProperty::MakeDefault<std::u32string>("Text",
	UIActionTrigger::Layout_Render).Register<TextBlock>(); 

TextBlock::TextBlock() noexcept :
	_GlyphBoxes(),
	_Brush(UIFactory<TextBrush>::Create())
{
}
TextBlock::~TextBlock() noexcept
{
}

void TextBlock::SetFont(const std::shared_ptr<Media::Font>& font) noexcept
{
	_Brush->SetFont(font);
}

Vector4 TextBlock::GetColor() const noexcept
{
	return _Brush->GetColor();
}
void TextBlock::SetColor(const Vector4& color) noexcept
{
	_Brush->SetColor(color);
}

const std::u32string& TextBlock::GetText() const noexcept
{
	return GetProperty<std::u32string>(TextProperty);
}
void TextBlock::SetText(const std::u32string& unicodes) noexcept
{
	_Brush->BuildCache(unicodes);
	_Brush->SetText(unicodes);
	SetProperty(TextProperty, unicodes);
}

Size TextBlock::Layout(const Size& size) noexcept
{
	auto rv = Control::Layout(size);
	UpdateGlyphMap();
	return rv;
}
void TextBlock::PreRender(const ICanvas& canvas) noexcept
{
	Control::PreRender(canvas);
}
void TextBlock::DoRender(const ICanvas& canvas) noexcept
{
	Control::DoRender(canvas);
	canvas->SetBrush(_Brush);
	canvas->DrawRects(_GlyphBoxes);
}

void TextBlock::UpdateGlyphMap() noexcept
{
	const auto& text = GetText();
	auto size = text.size();
	_GlyphBoxes = Array<Rect>(
		size);
	auto clientArea = GetSize();
	auto data = _GlyphBoxes.data();
	const auto& metrics = _Brush->GetCachedMetrics();
	auto pxSize = _Brush->GetFont()->GetPixelSize();

	int xPos = 0;
	int yPos = 0;

	for (auto i = 0; i < size; ++i)
	{
		const auto& metric = metrics.at(text.at(i));

		long newXPos = xPos + metric.Advance.X, newYPos = yPos;
		if (newXPos > clientArea.Width)
		{
			// yAdvance will be 0.
			newYPos = (yPos += pxSize);
			xPos = 0;
			newXPos = metric.Advance.X;
			if (newYPos > clientArea.Height)
				break;
		}

		Float xOffset = metric.Offset.X;
		Float yOffset = pxSize - metric.Offset.Y;
		
		auto& cur = data[i];
		cur.Left = xPos + xOffset;
		cur.Top = yPos + yOffset;
		cur.Right = cur.Left + pxSize;
		cur.Bottom = cur.Top + pxSize;

		xPos = newXPos;
		yPos = newYPos;
	}
}

_L_NS_END_GRAPHICS_UI_CONTROLS
