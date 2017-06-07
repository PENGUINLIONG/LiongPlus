#pragma once
#include "_"
#include "../../Interface.hpp"

_L_NS_BEG_GRAPHICS_UI

class DeviceContextHost
{
public:
	virtual DeviceContext GetDeviceContext() = 0;
};

using IDeviceContextHost = I<DeviceContextHost>;

_L_NS_END_GRAPHICS_UI
