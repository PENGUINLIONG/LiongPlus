// File: Window.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include <map>
#include <memory>
#include <string>
#include "_"

#include "UIFactory.hpp"

#include "ContentControl.hpp"
#include "../../Threading/Dispatcher.hpp"
#include "../../Event.hpp"
#include "../../Interface.hpp"
#include "UIActionHost.hpp"
#include "UIEvents.hpp"
#include "DeviceContextHost.hpp"

extern LRESULT __stdcall LiongPlus_Graphics_Window_WndProc
(HWND window, UINT msg, WPARAM w, LPARAM l);

_L_NS_BEG_GRAPHICS_UI

class Window;

using IWindow = Interface<Window>;

class Window :
	public ContentControl,
	public DeviceContextHost,
	public UIActionHost,
	public DependencyObject<Window>
{
	friend LRESULT __stdcall
		::LiongPlus_Graphics_Window_WndProc
		(HWND window, UINT msg, WPARAM w, LPARAM l);
private:
	enum
	{
		__MaxLengthForClassName = 256,
		__DefaultX = 150,
		__DefaultY = 100,
		__DefaultWidth = 750,
		__DefaultHeight = 750,
	};

	bool _Initialize();
	bool _Finalize();

public:
	static const UIProperty
		TopmostProperty,
		MouseInsideProperty,
		WindowContextProperty,
		CanvasProperty;
	
	Window();
	~Window();

	virtual void SetTitle(const std::string& title);
	ICanvas GetCanvas();
	virtual void SetCanvas(const ICanvas& canvas);

	void Show();
	void Hide();

	void OnSizeChanged(EventArgs& args) override;

	void PreRender(const ICanvas& canvas) override;
	void PostRender(const ICanvas& canvas) override;

	DeviceContext GetDeviceContext() override;
	void TriggerRender() override;
	void TriggerLayout() override;
	void TriggerLogicTree() override;

	static void EnterMessageLoop();
};

template<>
struct UIFactory<Window>
{
	static std::shared_ptr<Window> Create();
};

_L_NS_END_GRAPHICS_UI
