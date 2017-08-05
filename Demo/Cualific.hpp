// File: Cualific.hpp
// Author: Rendong Liang (Liong)
#include <map>
#include <memory>
#include <vector>
#include "LiongPlus/AnsiCharUtils.hpp"
#include "LiongPlus/Array.hpp"
#include "LiongPlus/Serialization/Xml.hpp"
#include "LiongPlus/Graphics/UI/UICommonDefs.hpp"
#include "LiongPlus/FileFormat/FileUtils.hpp"

using namespace std;
using namespace LiongPlus;
using namespace LiongPlus::FileFormat;
using namespace LiongPlus::Graphics::UI;
using namespace LiongPlus::Serialization;

struct CualificDefinition
{
	using Strokes = map<string, Array<Point>, greater<string>>;
	using AttachedStrokes = multimap<string, Strokes>;

	Strokes Major;
	AttachedStrokes Attached;
};

Point mapVertex(string code)
{
	switch (code[0])
	{
	case '1':
		return { 0., 0. };
	case '2':
		return { .5, 0. };
	case '3':
		return { 1., 0. };
	case '4':
		return { 0., 1. };
	case '5':
		return { 0.5, 1. };
	case '6':
		return { 1., 1. };
	case '7':
		return { 0., 2. };
	case '8':
		return { 0.5, 2. };
	case '9':
		return { 1., 2. };
	}
	return { -1., -1. };
}

void loadStrokes(const XmlElement& def, CualificDefinition::Strokes& majorStrokes)
{
	auto strokeCount = def.Children.size();
	auto literal = def.Attributes.at("Literal");
	Array<Point> strokes(2 * strokeCount);
	auto pos = 0;
	for (auto child : def.Children)
	{
		if (child->Name != "Stroke")
			continue;

		auto vertFrom = mapVertex(child->Attributes["From"]);
		strokes[pos * 2 + 0] = vertFrom;

		auto vertTo = mapVertex(child->Attributes["To"]);
		strokes[pos * 2 + 1] = vertTo;

		++pos;
	}
	majorStrokes.insert({ literal, strokes });
}
void loadAttachedStrokes(const XmlElement& def, CualificDefinition::AttachedStrokes& attachedStrokes)
{
	auto strokeCount = def.Children.size();
	Array<Point> strokes(2 * strokeCount);
	auto pos = 0;
	for (auto child : def.Children)
	{
		if (child->Name != "AttachedStroke")
			continue;
		auto it = attachedStrokes.insert({ def.Attributes.at("Literal"),{} });
		loadStrokes(*child, it->second);
	}
}

shared_ptr<CualificDefinition> getCualificDef()
{
	static shared_ptr<CualificDefinition> cualificDef;
	if (cualificDef == nullptr)
	{
		cualificDef = make_shared<CualificDefinition>();
		auto str = FileUtils::ReadAllToString("F:/CualificStrokeDefinition.xml");
		auto xml = XmlDocument::Parse(str);

		for (auto def : xml.Root().Children)
		{
			loadStrokes(*def, cualificDef->Major);
			loadAttachedStrokes(*def, cualificDef->Attached);
		}
	}
	return cualificDef;
}

vector<Point> translateToCualificVerticies(const string& str, const Size& clientArea, float scale)
{
	using namespace LiongPlus;
	vector<Point> result;
	auto def = getCualificDef();

	float xOffset = 0.f, yOffset = 0.f;

	for (auto index = 0; index < str.size(); ++index)
	{
		// Advance forward.
		float newXOffset = xOffset + 1.25f * scale,
			newYOffset = yOffset;
		if (newXOffset > clientArea.Width)
		{
			newYOffset = yOffset += 2.25f * scale;
			xOffset = 0;
			newXOffset = 1.25f * scale;
			if (newYOffset > clientArea.Height)
				return move(result);
		}

		string initial = "";
		for (const auto& majorDef : def->Major)
		{
			if (index + majorDef.first.length() > str.length())
				continue;
			auto ptr = majorDef.first.data();
			auto len = majorDef.first.length();
			if (!AnsiCharUtils::SectionalEquals(ptr, ptr + len, str.data() + index, str.data() + index + len))
				continue;
			initial = majorDef.first;
			break;
		}

		// Draw major strokes.
		for (auto val : def->Major[initial])
		{
			result.push_back({ val.X * scale + xOffset,
				val.Y * scale + yOffset });
		}
		auto range = def->Attached.equal_range(initial);
		auto& pos = range.first;
		auto& end = range.second;
		// Draw attached strokes.
		for (; pos != end; ++pos)
		{
			auto& attached = pos->second;
			auto& strokes = attached.begin();
			auto& literal = strokes->first;
			// If the window of a possible match gets out of the boundary of the original string, skip.
			if (index + literal.length() > str.length())
				continue;
			auto strBeg = str.data() + index;
			auto strEnd = str.data() + index + literal.length();
			if (!AnsiCharUtils::SectionalEquals(literal.begin(), literal.end(), strBeg, strEnd))
				continue;
			for (auto& val : strokes->second)
			{
				result.push_back({ val.X * scale + xOffset,
					val.Y * scale + yOffset });
			}
			index += literal.size() - 1;
			break;
		}
		xOffset = newXOffset;
		yOffset = newYOffset;
	}

	return move(result);
}

//auto verticies = translateToCualificVerticies("ABCDEFGHIJKLMNOPQRSTUVWXYZ", sender->GetSize(), 50);
//auto verticies = translateToCualificVerticies("ZHCHSH AIAOEIUI OUERIAIAOIEIU UAUAIUANUANGUO U..EU..UANU..N ANIANENINUN ANGIANGENGINGONGIONG", sender->GetSize(), 20);
