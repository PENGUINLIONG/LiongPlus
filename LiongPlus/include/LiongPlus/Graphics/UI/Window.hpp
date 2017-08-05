// File: Window.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include <map>
#include <string>
#include "_"
#include "UIFactory.hpp"
#include "ContentControl.hpp"
#include "UIActionHost.hpp"
#include "UIEvents.hpp"
#include "LiongPlus/MetadataHost.hpp"
#include "LiongPlus/Platform/MinWindows.hpp"

#ifdef __L_WINDOWS
extern LiongPlus::Platform::Windows::LResult __stdcall LiongPlus_Graphics_Window_WndProc(
	LiongPlus::Platform::Windows::Handle window,
	unsigned msg,
	LiongPlus::Platform::Windows::WParam w,
	LiongPlus::Platform::Windows::LParam l);
#endif // __L_WINDOWS

_L_NS_BEG_GRAPHICS_UI

class Window;

using IWindow = Interface<Window>;

class Window :
	public ContentControl,
	public MetadataHost,
	public UIActionHost,
	public DependencyObject<Window>
{
#ifdef __L_WINDOWS
	friend extern LiongPlus::Platform::Windows::LResult __stdcall ::LiongPlus_Graphics_Window_WndProc(
		LiongPlus::Platform::Windows::Handle window,
		unsigned msg,
		LiongPlus::Platform::Windows::WParam w,
		LiongPlus::Platform::Windows::LParam l);
#endif
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
		MouseInsideProperty,
		CanvasProperty;
	
	Window();
	~Window();

	virtual void SetTitle(const std::string& title);
	ICanvas GetCanvas();
	virtual void SetCanvas(const ICanvas& canvas);

	void Show() const;
	void Hide() const;

	void OnSizeChanged(EventArgs& args) override;

	void PreRender(const ICanvas& canvas) override;
	void PostRender(const ICanvas& canvas) override;

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
