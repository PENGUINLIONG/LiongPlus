
#include <array>
#include <iostream>
#include <fstream>

#include "LiongPlus/Serialization/Xml.hpp"
#include "LiongPlus/Testing/UnitTest.hpp"
#include "LiongPlus/Testing/Assert.hpp"

#include "LiongPlus/AnsiCharUtils.hpp"
#include "LiongPlus/Array.hpp"
#include "LiongPlus/FileFormat/FileUtils.hpp"
#include "LiongPlus/FileFormat/Bmp.hpp"
#include "LiongPlus/FileFormat/Png.hpp"
#include "LiongPlus/FileFormat/Targa.hpp"
#include "LiongPlus/Serialization/Base64.hpp"
#include "LiongPlus/Serialization/ColorLiteral.hpp"
#include "LiongPlus/Graphics/UI/Controls/Button.hpp"
#include "LiongPlus/Graphics/UI/Controls/Grid.hpp"
#include "LiongPlus/Graphics/UI/Controls/FullFramerate.hpp"
#include "LiongPlus/Graphics/UI/Animation.hpp"
#include "LiongPlus/Graphics/UI/Window.hpp"
#include "LiongPlus/Graphics/UI/OpenGLBrush.hpp"
#include "LiongPlus/Graphics/UI/OpenGLCanvas.hpp"
#include "LiongPlus/Graphics/UI/Controls/TextBlock.hpp"
#include "LiongPlus/Graphics/UI/Controls/Doodle.hpp"
#include "LiongPlus/Graphics/UI/UIUpdater.hpp"
#include "LiongPlus/Threading/Dispatcher.hpp"
#include "LiongPlus/Graphics/OpenGL/BufferObject.hpp"
#include "LiongPlus/Graphics/OpenGL/VertexArray.hpp"
#include "LiongPlus/Graphics/OpenGL/ShaderProgram.hpp"
#include "LiongPlus/Path.hpp"
#include "Pmx.hpp"

#include "LiongPlus/Runtime.hpp"

#include "LiongPlus/FileFormat/Bmp.hpp"
#include "LiongPlus/FileFormat/FileUtils.hpp"

using namespace std;
using namespace LiongPlus::Serialization;
using namespace LiongPlus::Testing;
using namespace LiongPlus::Graphics::UI;

void waitTilInput()
{
	using namespace std;
	char i;
	cin >> i;
}

static string colorConfiguration =
"<ColorConfig>"
"  <Color>#f35c87bb</Color>"
"  <Color>#34e2c0</Color>"
"  <Color>#0079d7aa</Color>"
"  <Color>#bbfff288</Color>"
"</ColorConfig>";

extern "C" __declspec(dllimport)
unsigned __stdcall glGetError();


unsigned GetGLError()
{
	return glGetError();
}

int main()
{
	//(XmlTest()).Test();
	//cout << UnitTest::Summary();

	using namespace std;
	using namespace std::chrono_literals;
	using namespace LiongPlus;
	using namespace LiongPlus::Threading;
	using namespace LiongPlus::Serialization;
	using namespace LiongPlus::Graphics;
	using namespace LiongPlus::Graphics::UI;
	using namespace LiongPlus::Graphics::UI::Controls;
	using namespace LiongPlus::Serialization;
	using namespace LiongPlus::Media;
	using namespace LiongPlus::FileFormat;

	DispatcherGuard dispatcher;
	UIUpdaterGuard guard;

	auto window = UIFactory<Window>::Create();
	auto canvas = UIFactory<OpenGLCanvas>::Create(window);
	window->SetCanvas(canvas);

	auto colorConfig = Serialization::XmlDocument::Parse(colorConfiguration);
	auto colors = colorConfig.Root().Children;
	auto magentaBrush = UIFactory<SolidBrush>::Create(
		(ColorLiteral::Parse(colors.front()->Content())));
	auto cyanBrush = UIFactory<SolidBrush>::Create(
		(ColorLiteral::Parse(colors.back()->Content())));

	auto imgFile = FileUtils::ReadAllToBuffer(R"(F:\ModelFiles\TDA Default Miku (Xoriu Ver.)\f.spa)");
	auto imageBrush = UIFactory<ImageBrush>::Create(
		Bmp::Resolve(imgFile.Field(), imgFile.Length()));

	auto grid = UIFactory<Grid>::Create();
	grid->SetMargin({ 50, 50, 50, 50 });
	grid->SetSize({ 400,400 });
	grid->SetColumnDefinitions({
		{ 0, GridMetricUnit::Auto },
		{ 20, GridMetricUnit::Percent },
	});
	grid->SetRowDefinitions({
		{ 0, GridMetricUnit::Auto },
		{ 20, GridMetricUnit::Percent },
	});

	Thickness from{ 50, 50, 50, 50 }, to{ 0, 0, 0, 0 };

	Animation anime{ AnimationTimeline{ 1000ms, true, -1 },
		std::make_shared<PowerEasingFunction>(5, EasingMode::EaseInOut),
		std::make_shared<ThicknessUpdater>(from, to) };

	auto button = UIFactory<Button>::Create();
	button->Click += [](const IUIElement& sender, MouseEventArgs& args)
	{
		cout << "Mouse click! At (" <<
			args.Position.X << ", " << args.Position.Y <<
			")." << endl;
		auto& window = sender.Get<Window>();
		auto border = window->GetProperty<Thickness>(Window::BorderProperty);
		auto size = window->GetProperty<Size>(window->SizeProperty);

		args.IsHandled = true;
	};

	button->SetSize({ 350, 350 });
	button->SetMargin({ 10, 10, 10, 10 });
	button->SetHorizontalAlignment(HorizontalAlignment::Stretch);
	button->SetVerticalAlignment(VerticalAlignment::Stretch);
	button->SetBackGround(imageBrush);

	window->SetTitle("LiongPlus Demo");
	window->SetCanvas(canvas);
	window->SetContent(grid);
	window->SetBackGround(cyanBrush);

	grid->SetBackGround(magentaBrush);
	grid->SetHorizontalAlignment(HorizontalAlignment::Stretch);
	grid->SetVerticalAlignment(VerticalAlignment::Stretch);

	window->MouseEnter += [](const IUIElement& sender, EventArgs& args)
	{ cout << "Mouse entered!" << endl; };
	window->MouseLeave += [](const IUIElement& sender, EventArgs& args)
	{ cout << "Mouse left!" << endl; };

	shared_ptr<Font> font = make_shared<Font>();
	font->Load(FileUtils::ReadAllToBuffer("F:/msyhl.ttc"));
	font->SetPixelSize(25);

	auto tb = make_shared<TextBlock>();
	tb->SetFont(font);
	tb->SetColor({ .4f, 1.f, .4f, 1.f });
	std::wstring text = L"Test 测试 テスト The quick brown fox jumped over the lazy dog. 一只敏捷的棕色狐狸跳过了那条懒狗。";
	tb->SetText(Unicode::FromUtf16(text.begin(), text.end()));
	tb->SetHorizontalAlignment(HorizontalAlignment::Stretch);
	tb->SetVerticalAlignment(VerticalAlignment::Stretch);
	tb->AttachProperty<Grid>();
	
	
	auto pmx = FileUtils::ReadAllToBuffer("F:/ModelFiles/TDA IYDD Gumi/TDA IYDD Gumi.pmx");
	Pmx::Resolve(pmx.Field(), pmx.Length());
	
	auto doodle = UIFactory<Doodle>::Create();
	doodle->SetHorizontalAlignment(HorizontalAlignment::Stretch);
	doodle->SetVerticalAlignment(VerticalAlignment::Stretch);
	doodle->SetMargin({ 10., 10. });
	doodle->SetBackGround(magentaBrush);
	doodle->Render += [&](const IUIElement& sender, const RenderEventArgs& args) {
		using namespace LiongPlus::Graphics::OpenGL;

		VertexArray arr;
		ArrayBuffer buf;
		UniformBuffer settings;
		UniformBuffer bones;
		ShaderProgram prog;

	};
	doodle->AttachProperty<Grid>();

	Animation::Apply(anime, UIElement::MarginProperty, grid);

	grid->SetChildren({ tb, doodle, button });

	string str = R"(F:\ModelFiles\TDA Default Miku (Xoriu Ver.)\f.spa)";
	Logger::Log(Path::GetDirectory(str));
	Logger::Log(Path::GetExtension(str));
	Logger::Log(Path::GetFileName(str));
	Logger::Log(Path::GetFileNameNoExt(str));
	
	window->Show();
	grid->SetColumn(tb, 1);
	grid->SetRow(doodle, 1);

	Window::EnterMessageLoop();

	waitTilInput();
}
