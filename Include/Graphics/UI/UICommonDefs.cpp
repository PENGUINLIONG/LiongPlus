// File: UICommonDefs.cpp
// Author: Rendong Liang (Liong)
#include "UICommonDefs.hpp"

_L_NS_BEG_GRAPHICS_UI

// Rect

Float Rect::Width() const
{
	return Right - Left;
}
Float Rect::Height() const
{
	return Bottom - Top;
}
Size Rect::Size() const
{
	return{ Width(), Height() };
}
Float Rect::X() const
{
	return Left;
}
Float Rect::Y() const
{
	return Left;
}
Point Rect::Position() const
{
	return{ Left, Top };
}
Point Rect::Center() const
{
	return{ Left + Width() / 2, Top + Height() / 2 };
}

_L_NS_END_GRAPHICS_UI
