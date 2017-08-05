#include "LiongPlus/Matrix4x4.hpp"
#include "LiongPlus/Path.hpp"
#include "LiongPlus/Runtime.hpp"

#include "LiongPlus/MemoryWriter.hpp"

#include "LiongPlus/FileFormat/FileUtils.hpp"

#include "LiongPlus/Graphics/OpenGL/BufferObject.hpp"
#include "LiongPlus/Graphics/OpenGL/ShaderProgram.hpp"
#include "LiongPlus/Graphics/OpenGL/Texture.hpp"
#include "LiongPlus/Graphics/OpenGL/VertexArray.hpp"

#include "LiongPlus/Graphics/UI/UIUpdater.hpp"
#include "LiongPlus/Graphics/UI/Window.hpp"
#include "LiongPlus/Graphics/UI/Controls/FullFramerate.hpp"
#include "LiongPlus/Graphics/UI/Controls/Doodle.hpp"

#include "LiongPlus/Serialization/Unicode.hpp"

#include "LiongPlus/Threading/Dispatcher.hpp"

#include "LiongPlus/Platform/MinOpenGL.hpp"

#include "Pmx.hpp"
#include "TextureReader.hpp"

using namespace std;
using namespace LiongPlus;
using namespace LiongPlus::FileFormat;
using namespace LiongPlus::Serialization;
using namespace LiongPlus::Threading;
using namespace LiongPlus::Graphics;
using namespace LiongPlus::Graphics::OpenGL;
using namespace LiongPlus::Graphics::UI;
using namespace LiongPlus::Graphics::UI::Controls;
using namespace LiongPlus::Platform::OpenGL;

auto vertStr()
{
	return
#include "MMDVertexShader.glsl"
		;
}
auto fragStr()
{
	return
#include "MMDFragmentShader.glsl"
		;
}

void main()
{
	DispatcherGuard dg;
	UIUpdaterGuard uiug;
	
	string filePath = "F:/ModelFiles/TDA IYDD Gumi/TDA IYDD Gumi.pmx";
	auto file = FileUtils::ReadAllToBuffer(filePath);

	auto pmx = Pmx::Resolve(file.Field(), file.Length());
	auto baseDir = Path::GetDirectory(filePath);
	Array<Texture<TextureType::Texture2D>> textures;
	long i = 0;
	for (auto& str : pmx.Textures)
	{
		auto img = ReadImageFile(baseDir, Unicode::ToUtf8(str.begin(), str.end()));
		textures[i].Allocate(img.Format, img.Width, img.Height);
		textures[i].Write(img.Data.Field(), img.Format);
	}

	Shader vert(ShaderType::Vertex), frag(ShaderType::Fragment);
	vert.Compile(vertStr());
	frag.Compile(fragStr());
	ShaderProgram prog;
	prog.Attach(vert);
	prog.Attach(frag);
	prog.Link();
	prog.Detach(vert);
	prog.Detach(frag);
	VertexArray vao = prog.InitVertexArray();
	ArrayBuffer arrBuf;
	auto bufSize = pmx.Vertices.size() * (4 + 4 + 4 + 1 + 8 + 1) * 4;
	arrBuf.Allocate(bufSize, false);
	auto ref = BufferPool::Ginst().Get();
	auto buf = *ref;
	{
		MemoryWriter writer(buf.Field(), bufSize);
		for (auto& vertex : pmx.Vertices)
		{
			writer.TryWriteManyByIndex<float>(0, 3, vertex.Position);
			writer.TryWriteManyByIndex<float>(0, 3, vertex.Normal);
			writer.TryWriteManyByIndex<float>(0, 4, vertex.UV);
			
			writer.TryWrite<int32_t>(vertex.DeformType);
			
			writer.TryWrite<int32_t>(vertex.DeformData.Bone1Index);
			writer.TryWrite<int32_t>(vertex.DeformData.Bone2Index);
			writer.TryWrite<int32_t>(vertex.DeformData.Bone3Index);
			writer.TryWrite<int32_t>(vertex.DeformData.Bone4Index);
			
			writer.TryWrite<float>(vertex.DeformData.Bone1Weight);
			writer.TryWrite<float>(vertex.DeformData.Bone2Weight);
			writer.TryWrite<float>(vertex.DeformData.Bone3Weight);
			writer.TryWrite<float>(vertex.DeformData.Bone4Weight);

			writer.TryWriteManyByIndex<float>(0, 3, vertex.DeformData.C);
			writer.TryWriteManyByIndex<float>(0, 3, vertex.DeformData.R0);
			writer.TryWriteManyByIndex<float>(0, 3, vertex.DeformData.R1);
		}
	}
	arrBuf.Write(buf.Field());
	ElementArrayBuffer elemArrBuf;
	arrBuf.Allocate(pmx.Indices.size() * sizeof(int32_t), false);
	arrBuf.Write(pmx.Indices.data());
	UniformBuffer bonesBuf = prog.InitUniformBlock("Bones", nullptr);

	auto window = UIFactory<Window>::Create();
	auto doodle = UIFactory<Doodle>::Create();
	window->SetContent(doodle);

	doodle->Render += [&](const IUIElement& sender, RenderEventArgs& args)
	{
		glDrawElements(GL_TRIANGLES, pmx.Indices.size(), GL_UNSIGNED_INT, 0);
	};
	
	Logger::Log(Path::ToAbsolutePath<std::string>("/usr/include", "./../index.html"));

	window->Show();

	Window::EnterMessageLoop();
}
