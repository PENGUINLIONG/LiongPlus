// File: XmlDocument.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include <cstdint>
#include <stack>
#include "_"
#include "../SafeError.hpp"
#include "XmlElement.hpp"

_L_NS_BEG_XML

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

_L_NS_END_XML
