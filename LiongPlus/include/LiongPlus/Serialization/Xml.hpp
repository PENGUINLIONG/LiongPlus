// File: Xml.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include <stack>
#include <list>
#include <map>
#include <memory>
#include <string>
#include "_"
#include "../SafeError.hpp"

_L_NS_BEG_SERIALIZATION

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

enum XmlParseErrorCode : uint_fast16_t
{
	Success_Perfect             = 0b0'000'0000'0000'0000,
	Success_EmptyElementTag     = 0b0'000'0000'0000'0001,

	Error_Mask                  = 0b1'000'0000'0000'0000,
	// General XML errors.
	Error_DirtyOutputTarget     = 0b1'000'0000'0000'0001,
	Error_MultipleRootElement   = 0b1'000'0000'0000'0010,
	Error_UnexpectedChar        = 0b1'000'0000'0000'0011,
	// Tag-related errors.
	Error_TagUnclosed           = 0b1'000'0001'0000'0001,
	Error_TagNameNotFound       = 0b1'000'0001'0000'0010,
	Error_TagNameMismatched     = 0b1'000'0001'0000'0011,
	Error_EndTagNotFound        = 0b1'000'0001'0000'0100,
	Error_UnexpectedEndTag      = 0b1'000'0001'0000'0101,
	// Attribute-related errors.	
	Error_InvalidAttrName       = 0b1'000'0010'0000'0001,
	Error_InvalidAttrField      = 0b1'000'0010'0000'0010,
	Error_AttrAlreadyExist      = 0b1'000'0010'0000'0011,
	// Not implemented yet.
	Error_NotImplemented        = 0b1'111'1111'1111'1111,
};

using XmlParseError = SafeError<XmlParseErrorCode, uint_fast16_t>;

class XmlDocument
{
private:
	struct _XmlDocInfo
	{
		std::string Encoding;
		std::tuple<int, int> XmlVersion;
	};

	std::shared_ptr<XmlElement> _Root;
	std::shared_ptr<_XmlDocInfo> _Info;

	/*
	* Parse attribute list.
	* Assertion:
	*   $attributes: the map to filled.
	*   $pos: At a whitespace or at the end.
	*   $end: At '>'.
	* Returns:
	*   An error code.
	*/
	XmlParseError _ParseAttributes(std::map<std::string, std::string>& attributes, std::string::const_iterator pos, const std::string::const_iterator end) noexcept;
	/*
	* Parse a simple tag (tag that closed directly). e.g., <Tag />
	* Params:
	*   $elemStack: Element stack used to represent the layer  structure of XML.
	*   $pos: At the beg of name.
	*   $end: At '>'.
	* Returns:
	*   An error code.
	*/
	XmlParseError XmlDocument::_ParseSimpleTag(std::stack<std::shared_ptr<XmlElement>>& elemStack, std::string::const_iterator& pos, std::string::const_iterator& end) noexcept;
	/*
	* Parse a tag.
	* Params:
	*   $pos: At the first non-whitespace char.
	*   $end: At '>'.
	* Returns:
	*   An error code.
	*/
	XmlParseError _ParseTag(std::stack<std::shared_ptr<XmlElement>>& elemStack, std::string::const_iterator& pos, std::string::const_iterator& end) noexcept;

protected:
	/*
	* Parse a functional instruction. e.g., <!Document html>
	* Params:
	*   $pos: At '!'.
	*   $end: At '>'.
	* Returns:
	*   An error code.
	*/
	XmlParseError RecordFunctionalInstruction(std::string::const_iterator& pos, std::string::const_iterator& end) noexcept;
	/*
	* Parse a processing instruction. e.g., <?php ?>
	* Params:
	*   $pos: At the first '?'.
	*   $end: At '>'.
	* Returns:
	*   An error code.
	*/
	XmlParseError RecordProcessingInstruction(std::string::const_iterator& pos, std::string::const_iterator& end) noexcept;

public:
	XmlDocument() noexcept;
	XmlDocument(const XmlDocument& instance) noexcept;
	XmlDocument(XmlDocument&& instance) noexcept;

	XmlDocument& operator=(const XmlDocument&) noexcept = default;
	XmlDocument& operator=(XmlDocument&& instance) noexcept;

	XmlElement& Root() noexcept;
	const XmlElement& Root() const noexcept;
	std::string& Encoding() noexcept;
	const std::string& Encoding() const noexcept;
	std::tuple<int, int>& XmlVersion() noexcept;
	const std::tuple<int, int>& XmlVersion() const noexcept;

	static XmlDocument Parse(const std::string& xml);
	static XmlParseError TryParse(XmlDocument& _Doc, const std::string& xml) noexcept;
};

_L_NS_END_SERIALIZATION
