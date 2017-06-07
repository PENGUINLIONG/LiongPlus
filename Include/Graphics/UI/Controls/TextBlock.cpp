// File: TextBlock.cpp
// Author: Rendong Liang (Liong)
#include "TextBlock.hpp"

_L_NS_BEG_GRAPHICS_UI_CONTROLS

const UIProperty TextBlock::TextProperty =
UIProperty::MakeDefault<std::vector<uint32_t>>("Text",
	UIActionTrigger::Render).Register<TextBlock>(); 

TextBlock::TextBlock() noexcept :
	_NeedFontUpdate(false),
	_VertexBuffer(),
	_Brush(std::make_shared<TextBrush>())
{
}
TextBlock::~TextBlock() noexcept
{
}

void TextBlock::SetFont(const std::shared_ptr<Media::Font>& font) noexcept
{
	_Brush->SetFont(font);
}

const std::vector<uint32_t>& TextBlock::GetText() const noexcept
{
	return GetProperty<std::vector<uint32_t>>(TextProperty);
}
void TextBlock::SetText(const std::string& text)
{
	std::vector<uint32_t> unicodes;
	auto i = Serialization::Unicode::FromUtf8(
		text.begin(), text.end(), unicodes);
	if (i.IsSuccess())
	{
		_Brush->BuildCache(unicodes);
		_Brush->SetText(unicodes);
		SetProperty(TextProperty, std::move(unicodes));
		_NeedFontUpdate = true;
	}
	else throw std::runtime_error("Unable to decode UTF-8.");
}

void TextBlock::PreRender(const ICanvas& canvas) noexcept
{
	Control::PreRender(canvas);
	if (_NeedFontUpdate)
		UpdateGlyphMap();
}
void TextBlock::DoRender(const ICanvas& canvas) noexcept
{
	Control::DoRender(canvas);
	canvas->SetBrush(_Brush);
	canvas->DrawDirectly(_VertexBuffer.data(),
		_VertexBuffer.size() / 2);
}

void TextBlock::UpdateGlyphMap() noexcept
{
	const auto& text = GetText();
	auto size = text.size();
	_VertexBuffer = Array<float>(
		size * 2 /* 1x2 vector */ * 6 /* 6 verticies */);
	auto clientArea = GetSize();
	auto data = _VertexBuffer.data();
	const auto& metrics = _Brush->GetCachedMetrics();
	auto pxSize = _Brush->GetFont()->GetPixelSize();

	int xPos = 0;
	int yPos = 0;

	for (auto i = 0; i < size; ++i)
	{
		const auto& metric = metrics.at(text.at(i));
		auto xOffset = metric.Offset.X;
		auto yOffset = pxSize - metric.Offset.Y;
		// Upper-right.
		data[i * 12 + 0] = xPos + xOffset + pxSize;
		data[i * 12 + 1] = yPos + yOffset;
		// Upper-left.
		data[i * 12 + 2] = xPos + xOffset;
		data[i * 12 + 3] = yPos + yOffset;
		// Lower-left.
		data[i * 12 + 4] = xPos + xOffset;
		data[i * 12 + 5] = yPos + yOffset + pxSize;

		data[i * 12 + 6] = xPos + xOffset;
		data[i * 12 + 7] = yPos + yOffset + pxSize;
		// Lower-right.
		data[i * 12 + 8] = xPos + xOffset + pxSize;
		data[i * 12 + 9] = yPos + yOffset + pxSize;
		// Upper-right.
		data[i * 12 + 10] = xPos + xOffset + pxSize;
		data[i * 12 + 11] = yPos + yOffset;

		xPos += metric.Advance.X;
		if (xPos > clientArea.Width)
		{
			// yAdvance will be 0.
			yPos += pxSize;
			xPos = 0;
			if (yPos > clientArea.Height)
				break;
		}
	}
}

_L_NS_END_GRAPHICS_UI_CONTROLS
