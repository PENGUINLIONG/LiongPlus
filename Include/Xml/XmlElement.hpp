// File: XmlElement.hpp
// Author: Rendong Liang (Liong)
#pragma once

#include <list>
#include <map>
#include <memory>
#include <string>
#include "_"

_L_NS_BEG_XML

struct XmlElementContentSection
{
public:
	std::unique_ptr<XmlElementContentSection> Next;
	std::string Content;

	/*
	 * Get the aggregation of the current and the following sections.
	 * Return:
	 *   The aggregation of the current and the following sections.
	 */
	std::string GetContent();
	/*
	 * Append a new content section at the end of the section chain.
	 * Params:
	 *   $str: The string of section to be added.
	 * Returns:
	 *   The newly added section object.
	 */
	XmlElementContentSection& AppendContent(std::string&& str);
};

class XmlDocument;

class XmlElement
{
	friend XmlDocument;
private:
	std::weak_ptr<XmlElement> _Parent;
	std::weak_ptr<XmlElement> _Previous;
	std::weak_ptr<XmlElement> _Next;
	std::unique_ptr<XmlElementContentSection> _Content;

public:
	std::string Name;
	std::map<std::string, std::string> Attributes;
	std::list<std::shared_ptr<XmlElement>> Children;

	XmlElement();
	XmlElement(const XmlElement&) = delete;
	XmlElement(XmlElement&& instance);

	XmlElement& operator=(const XmlElement&) = default;
	XmlElement& operator=(XmlElement&& instance);

	std::string Content();
	std::shared_ptr<XmlElement> Parent();
	std::shared_ptr<XmlElement> Previous();
	std::shared_ptr<XmlElement> Next();
	
	XmlElementContentSection& AppendContent(std::string&& str);
};

_L_NS_END_XML
