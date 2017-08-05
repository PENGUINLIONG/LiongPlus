// File: TextureReader.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include "LiongPlus/FileFormat/Bmp.hpp"
#include "LiongPlus/FileFormat/Targa.hpp"
#include "LiongPlus/FileFormat/Png.hpp"
#include "LiongPlus/FileFormat/FileUtils.hpp"
#include "LiongPlus/Path.hpp"

using namespace LiongPlus;
using namespace LiongPlus::Media;
using namespace FileFormat;

Image ReadImageFile(const std::string& baseDir, const std::string& path)
{
	auto ext = Path::GetExtension(path);
	auto absPath = Path::ToAbsolutePath(baseDir, path);

	auto content = FileUtils::ReadAllToBuffer(absPath);

	if (ext == "bmp" || ext == "spa")
		return FileFormat::Bmp::Resolve(content.Field(), content.Length());
	else if (ext == "png")
		return FileFormat::Png::Resolve(content.Field(), content.Length());
	else if (ext == "tga" || ext == "targa")
		return FileFormat::Targa::Resolve(content.Field(), content.Length());
	else return {};
}
