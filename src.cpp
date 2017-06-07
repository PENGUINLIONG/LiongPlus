
#include <array>
#include <iostream>
#include <fstream>
#include <gl\glew.h>
#include "Include/Xml/XmlDocument.hpp"
#include "Include/Testing/UnitTest.hpp"
#include "Include/Testing/Assert.hpp"

#include "Include/FileFormat/FileUtils.hpp"
#include "Include/FileFormat/Bmp.hpp"
#include "Include/Serialization/Base64.hpp"
#include "Include/Serialization/ColorLiteral.hpp"
#include "Include/Graphics/ClampColor.hpp"
#include "Include/Graphics/UI/Controls/Button.hpp"
#include "Include/Graphics/UI/Controls/Grid.hpp"
#include "Include/Graphics/UI/Animation.hpp"
#include "Include/Graphics/UI/Window.hpp"
#include "Include/Graphics/UI/OpenGLBrush.hpp"
#include "Include/Graphics/UI/OpenGLCanvas.hpp"
#include "Include/Graphics/UI/Controls/TextBlock.hpp"
#include "Include/Threading/Dispatcher.hpp"

#include "Include/FileFormat/Bmp.hpp"
#include "Include/FileFormat/FileUtils.hpp"

using namespace std;
using namespace LiongPlus::Xml;
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

GLenum GLGetError()
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
	using namespace LiongPlus::Xml;
	using namespace LiongPlus::Graphics;
	using namespace LiongPlus::Graphics::UI;
	using namespace LiongPlus::Graphics::UI::Controls;
	using namespace LiongPlus::Serialization;
	using namespace LiongPlus::Media;
	using namespace LiongPlus::FileFormat;

	RegisterDispatcher reg;

	auto window = UIFactory<Window>::Create();
	auto canvas = UIFactory<OpenGLCanvas>::Create(window);
	window->SetCanvas(canvas);
	
	auto colorConfig = Xml::XmlDocument::Parse(colorConfiguration);
		auto colors = colorConfig.Root().Children;
	auto magentaBrush = UIFactory<SolidBrush>::Create(ClampColor(ColorLiteral::Parse(colors.front()->Content())));
	auto cyanBrush = UIFactory<SolidBrush>::Create(ClampColor(ColorLiteral::Parse(colors.back()->Content())));
	
	//string cxpath;
	//cout << "Where is chunxu? Input the path to cx.bmp: ";
	//cin >> cxpath;
	//cout << "Good." << endl;
	//auto imgFile = FileUtils::ReadAllToBuffer(cxpath);
	auto imgFile = FileUtils::ReadAllToBuffer("F:/cx.bmp");

	auto imageBrush = UIFactory<ImageBrush>::Create(
		Bmp::Resolve(imgFile.Field(), imgFile.Length()));

	auto grid = UIFactory<Grid>::Create();
	grid->SetMargin({ 50, 50, 50, 50 });
	grid->SetSize({ 400,400 });
	grid->SetColumnDefinitions({
		{ 0, GridMetricUnit::Auto },
		{ 10, GridMetricUnit::Percent },
	});

	Thickness from{ 50, 50, 50, 50 }, to{ 100, 0, 0, 0 };

	Animation anime{ AnimationTimeline{ 1000ms, true, -1 }, std::make_shared<PowerEasingFunction>(5, EasingMode::EaseInOut), std::make_shared<ThicknessUpdater>(from, to) };

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
	button->SetMargin({ 10, 15, 20, 80 });
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
	font->SetPixelSize(20);

	auto tb = make_shared<TextBlock>();
	tb->SetFont(font);
	auto text = FileUtils::ReadAllToString("F:/text.txt");
	tb->SetText(text);
	tb->SetHorizontalAlignment(HorizontalAlignment::Stretch);
	tb->SetVerticalAlignment(VerticalAlignment::Stretch);
	
	grid->SetChildren({ button, tb });

	Animation::Apply(anime, UIElement::MarginProperty, grid);
	sizeof(Animation);
	
	window->Show();
	/*
	// GL -----
	//

	GLuint vbo, vao, prog, vertsh, fragsh, uniblock;

	float verticies[] ={
		-1., 1., // top left
		1., 1., // top right
		1., -1., // bot left
		-1., -1.// bot right
	};
	const char* vert =
		"#version 400 core\n"
		"layout(std140) uniform Transform {"
		"   mat4 transform;"
		"};"
		"layout(location = 0) in vec2 vertexPos;"
		"void main()"
		"{"
		"   gl_Position = transform * vec4(vertexPos, 0., 1.);"
		"   gl_Position = vec4(vertexPos, 0., 1.);"
		"}";
	const char* frag =
		"#version 400 core\n"
		"layout(std140) uniform SolidSettings {"
		"	vec4 solidColor;"
		"};"
		"out vec4 colorOut;"
		"void main()"
		"{"
		"	colorOut = solidColor;"
		"	colorOut = vec4(1., 1., 1., 1.);"
		"}";
	GLint vertlen = sizeof(vert),
		fraglen = sizeof(frag);

	vertsh = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertsh, 1, &vert, &vertlen);
	glCompileShader(vertsh);
	
	fragsh = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragsh, 1, &frag, &fraglen);
	glCompileShader(fragsh);

	prog = glCreateProgram();
	glAttachShader(prog, vertsh);
	glAttachShader(prog, fragsh);
	glLinkProgram(prog);
	glDetachShader(prog, vertsh);
	glDetachShader(prog, fragsh);

	glDeleteShader(vertsh);
	glDeleteShader(fragsh);

	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_DYNAMIC_DRAW);

	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(float) * 2, 0);

	glUseProgram(prog);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	SwapBuffers(window->GetDeviceContext());

	//
	// GL -----


	*/
	
	Window::EnterMessageLoop();
	

	

	waitTilInput();
}