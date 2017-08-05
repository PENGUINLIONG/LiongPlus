// File: PictureFrame.hpp
// Author: Rendong Liang (Liong)
#include "_"
#include "../Control.hpp"
#include "../../../Media/Image.hpp"

_L_NS_BEG_GRAPHICS_UI_CONTROLS

enum class ImageScaling
{
	// Fit the drawable area. Ensure all parts of the image are
	//   visible.
	Fit,
	// Fill the drawable area. Ensure all parts of the drawable area
	//   are covered. Parts cannot be included will be clipped.
	Fill,
	// Stretch to fill the drawable area. Ensure all parts of the
	//   drawable area can be covered. The image boundaries attach to
	//   the container.
	Stretch,
};

class PictureFrame :
	public Control
{
public:
	const static UIProperty
		ImageProperty,
		ImageScalingProperty;

	Media::Image GetImage() const noexcept;
	void SetImage(const Media::Image& image) const noexcept;
	ImageScaling GetImageScaling() const noexcept;
	void SetImageScaling(ImageScaling scaling) noexcept;

	PictureFrame();
};

_L_NS_END_GRAPHICS_UI_CONTROLS
