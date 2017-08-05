// File: XmlElement.cpp
// Author: Rendong Liang (Liong)
#include <stack>
#include "LiongPlus/Serialization/Xml.hpp"
#include "LiongPlus/AnsiCharUtils.hpp"

_L_NS_BEG_SERIALIZATION

using namespace std;

//
// XmlElementContentSection
//


string XmlElementContentSection::GetContent()
{
	if (Next == nullptr) return Content;
	else return Content + Next->GetContent();
}

XmlElementContentSection& XmlElementContentSection::AppendContent(string&& str)
{
	if (Next == nullptr)
		return Next = make_unique<XmlElementContentSection>
		(XmlElementContentSection{ nullptr, str }), *this;
	else return Next->AppendContent(forward<string>(str));
}


//
// XmlElement
//


XmlElement::XmlElement()
	: _Parent()
	, _Previous()
	, _Next()
	, _Content()
	, Name()
	, Attributes()
	, Children()
{
}
XmlElement::XmlElement(XmlElement&& instance)
	: _Parent(move(instance._Parent))
	, _Previous(move(instance._Previous))
	, _Next(move(instance._Next))
	, _Content(move(instance._Content))
	, Name(move(instance.Name))
	, Attributes(move(instance.Attributes))
	, Children(move(instance.Children))
{
}

XmlElement& XmlElement::operator=(XmlElement&& instance)
{
	_Parent = move(instance._Parent);
	_Previous = move(instance._Previous);
	_Next = move(instance._Next);
	_Content = move(instance._Content);
	Name = move(instance.Name);
	Attributes = move(instance.Attributes);
	Children = move(instance.Children);
	return *this;
}

string XmlElement::Content() { return _Content ? _Content->GetContent() : ""; }

inline shared_ptr<XmlElement> tryLock(weak_ptr<XmlElement>& ptr)
{
	return ptr.expired() ? nullptr : ptr.lock();
}

shared_ptr<XmlElement> XmlElement::Parent() { return tryLock(_Parent); }
shared_ptr<XmlElement> XmlElement::Previous() { return tryLock(_Previous); }
shared_ptr<XmlElement> XmlElement::Next() { return tryLock(_Next); }

XmlElementContentSection& XmlElement::AppendContent(std::string&& str)
{
	if (_Content == nullptr) return *(_Content = make_unique<XmlElementContentSection>
		(XmlElementContentSection{ nullptr, str }));
	else return _Content->AppendContent(forward <string>(str));
}


//
// XmlDocument
//


namespace details
{
	constexpr bool isIgnorable(const char c) noexcept
	{
		using namespace AnsiCharUtils;
		return IsLineBreak(c) || IsWhiteSpace(c);
	}
	string::const_iterator seekForIgnorable(string::const_iterator pos, string::const_iterator end) noexcept
	{
		return AnsiCharUtils::SeekFor(pos, end, isIgnorable);
	}
	string::const_iterator seekForNonIgnorable(string::const_iterator pos, string::const_iterator end) noexcept
	{
		return AnsiCharUtils::SeekFor(pos, end, [](const char c) { return !isIgnorable(c); });
	}
}

XmlDocument::XmlDocument() noexcept
	: _Root()
	, _Info(new _XmlDocInfo{ "",{ 1, 0 } })
{
}
XmlDocument::XmlDocument(const XmlDocument& instance) noexcept
	: _Root(instance._Root)
	, _Info(instance._Info)
{
}
XmlDocument::XmlDocument(XmlDocument&& instance) noexcept
	: _Root(std::exchange(instance._Root, nullptr))
	, _Info(std::exchange(instance._Info, nullptr))
{
}

XmlDocument& XmlDocument::operator=(XmlDocument&& instance) noexcept
{
	_Root = std::exchange(instance._Root, nullptr);
	_Info = std::exchange(instance._Info, nullptr);
	return *this;
}

XmlElement& XmlDocument::Root() noexcept { return *_Root; }
const XmlElement& XmlDocument::Root() const noexcept { return *_Root; }

string& XmlDocument::Encoding() noexcept { return _Info->Encoding; }
const string& XmlDocument::Encoding() const noexcept { return _Info->Encoding; }

tuple<int, int>& XmlDocument::XmlVersion() noexcept { return _Info->XmlVersion; }
const tuple<int, int>& XmlDocument::XmlVersion() const noexcept { return _Info->XmlVersion; }

/*

1. Find '<'
2. Find '>'
3. Capture text in brackets
3.a Find '/'
3.a.TRUE Pop stack
3.a.FALSE Continue 3.b & 3.c
3.b Capture mark-up name
3.c Capture attributes
3.d Find '/' at the end
3.d.TRUE End up current element
3.d.FALSE Push stack
4. Record content
4.a If it is directly closed, end up current element
4.b If it is pure-content, record and replace all '@' with "@@"
4.c If it is mixed-content, record, replace all '@' with "@@",
replace where children are with '@' and record children

*/

XmlParseError XmlDocument::_ParseAttributes(map<string, string>& attributes, string::const_iterator pos, const string::const_iterator end) noexcept
{
	using namespace details;
	for (;;)
	{
		// Attribute name logic.

		auto pos_BegAttrName = seekForNonIgnorable(pos, end);
		if (*pos_BegAttrName == '/')
		{
			// Success: It is a empty-element tag.
			if (pos_BegAttrName + 1 == end) return Success_EmptyElementTag;
			else return Error_UnexpectedChar;
		}
		// Success: There is no attributes remaining.
		if (pos_BegAttrName == end) return Success_Perfect;
		auto pos_EndAttrName = AnsiCharUtils::SeekForChar(pos_BegAttrName, end, '=');
		// Error: Unable to find attribute name.
		if (pos_BegAttrName == pos_EndAttrName) return Error_InvalidAttrName;
		auto attrName = string(pos_BegAttrName, pos_EndAttrName);
		// Error: There is whitespace in attribute name.
		if (attrName.find(' ') != string::npos) return Error_InvalidAttrName;

		// Attribute field logic.

		auto pos_BegAttrField = pos_EndAttrName + 1;
		// Error: Attribute field absent.
		if (pos_BegAttrField == end) return Error_InvalidAttrField;
		string::const_iterator pos_EndAttrField;
		if (*pos_BegAttrField == '\'') pos_EndAttrField = AnsiCharUtils::SeekForChar(pos_BegAttrField + 1, end, '\'');
		else if (*pos_BegAttrField == '\"') pos_EndAttrField = AnsiCharUtils::SeekForChar(pos_BegAttrField + 1, end, '\"');
		// Error: Attribute field is quoted by a invalid character.
		else return Error_InvalidAttrField;
		// Error Attribute field is not closed by a corresponding quote.
		if (pos_EndAttrField == end) return Error_InvalidAttrField;
		auto attrField = string(pos_BegAttrField + 1, pos_EndAttrField);

		// Error: Attribute name already exists.
		if (attributes.find(attrName) != attributes.end())
			return Error_AttrAlreadyExist;

		// Success: Attribute field is recorded.
		attributes[attrName] = attrField;

		// Range checked: ($pos_EndAttrField + 1) is next to '\"' or '\'' at least at '>'.
		pos = pos_EndAttrField + 1;
	}
	return Success_Perfect;
}

XmlParseError XmlDocument::_ParseSimpleTag(stack<shared_ptr<XmlElement>>& elemStack, string::const_iterator& pos, string::const_iterator& end) noexcept
{
	using namespace details;

	auto elem = make_shared<XmlElement>();

	if (elemStack.empty())
	{
		if (_Root == nullptr) _Root = elem;
		// Error: Multiple root elements present.
		else return Error_MultipleRootElement;
	}
	// If current element is not the root element, bind relationship.
	else
	{
		auto top = elemStack.top();
		// If there are other children already in stack, bind relationship with the last child.
		if (!top->Children.empty())
		{
			auto pre = top->Children.back(); // The previous element.
			pre->_Next = elem;
			elem->_Previous = pre;
		}
		elem->_Parent = top;
		top->Children.push_back(elem); // Push the new element into stack.
	}
	elemStack.push(elem);
	// Parse attributes.
	auto pos_EndName = seekForIgnorable(pos, end);
	XmlParseError err = _ParseAttributes(elem->Attributes, pos_EndName, end);
	// Error: Internal error occurred.
	if (!err.IsSuccess()) return err;
	// Range checked: ($pos_EndName - 1) will at least at '<'
	if (Success_EmptyElementTag == err)
		elem->Name ={ pos, pos_EndName }, elemStack.pop();
	// '/' might be considered as a part of name.
	else if ('/' == *(pos_EndName - 1))
		elem->Name ={ pos, pos_EndName - 1 }, elemStack.pop();
	else // The element is not closed! Do not pop!
		elem->Name ={ pos, pos_EndName };
	// Finish: No error occurred yet.
	return Success_Perfect;
}

XmlParseError XmlDocument::RecordFunctionalInstruction(string::const_iterator& pos, string::const_iterator& end) noexcept
{
	return Error_NotImplemented;
}

XmlParseError XmlDocument::RecordProcessingInstruction(string::const_iterator& pos, string::const_iterator& end) noexcept
{
	return Error_NotImplemented;
}

XmlParseError XmlDocument::_ParseTag(stack<shared_ptr<XmlElement>>& elemStack, string::const_iterator& pos, string::const_iterator& end) noexcept
{
	using namespace details;
	// Range checked: ($pos + 1) is at least at '>' or $end.
	auto pos_BegName = seekForNonIgnorable(pos + 1, end);
	// Error: Unble to find tag name.
	if (pos_BegName == end) return Error_TagNameNotFound;

	auto leadingChar = *pos_BegName;
	if ('/' == leadingChar)
	{
		++pos_BegName;
		// In case of: </ TagName>
		if (isIgnorable(*pos_BegName))
			pos_BegName = seekForNonIgnorable(pos_BegName, end);
		auto pos_EndName = seekForIgnorable(pos_BegName, end);
		string name ={ pos_BegName, pos_EndName };
		// Error: Unexpected end tag,
		if (elemStack.empty()) return Error_UnexpectedEndTag;
		if (name == elemStack.top()->Name)
		{
			elemStack.pop();
			// Error: Followed by non-whitespace.
			if (seekForNonIgnorable(pos_EndName, end) != end)
				return Error_UnexpectedChar;
		}
		// Error: Tag names in begin- and end-tag mismatched.
		else return Error_TagNameMismatched;
	}
	else if ('?' == leadingChar) // Processing instruction.
								 // Range checked: ($pos_BegName + 1) is at least at '>'.
		RecordProcessingInstruction(pos + 1, end);
	else if ('!' == leadingChar) // Functional instruction.
								 // Range checked: ($pos_BegName + 1) is at least at '>'.
		RecordFunctionalInstruction(pos + 1, end);
	else // Just a simple element tag.
	{
		XmlParseError err = _ParseSimpleTag(elemStack, pos_BegName, end);
		// Error: Internal error occurred.
		if (!err.IsSuccess()) return err;
	}
	// Finish: No error occurred yet.
	return Success_Perfect;
}

string::const_iterator SeekForEndBracket(string::const_iterator pos, string::const_iterator end) noexcept
{
	bool isInAttrSingleQuote = false;
	bool isInAttrDoubleQuote = false;
	while (pos != end)
	{
		if ('>' == *pos)
		{
			if (isInAttrSingleQuote) continue;
			else if (isInAttrDoubleQuote) continue;
			else return pos;
		}
		else if ('\'' == *pos)
		{
			if (isInAttrSingleQuote) isInAttrSingleQuote = false;
			else if (!isInAttrDoubleQuote) isInAttrSingleQuote = true;
		}
		else if ('\"' == *pos)
		{
			if (isInAttrDoubleQuote) isInAttrDoubleQuote = false;
			else if (!isInAttrSingleQuote) isInAttrDoubleQuote= true;
		}
		++pos;
	}
	return pos;
}

XmlDocument XmlDocument::Parse(const std::string& xml)
{
	XmlDocument doc;
	XmlParseError err = TryParse(doc, xml);
	if (!err.IsSuccess())
		throw runtime_error(("Parse failed with code:" + std::to_string(err)).c_str());
	return doc;
}

XmlParseError XmlDocument::TryParse(XmlDocument& _Doc, const string& xml) noexcept
{
	using namespace AnsiCharUtils;

	if (nullptr != _Doc._Root)
		return Error_DirtyOutputTarget;

	auto pos = xml.begin();
	auto end = xml.end();
	auto pos_LastEndBracket = end;
	stack<shared_ptr<XmlElement>> elemStack;
	XmlParseError err = Success_Perfect;

	for (;;)
	{
		if (pos == end)
		{
			// Finish: At the end of specified text, return.
			if (elemStack.empty()) return err;
			// Error: Unexpected end of string.
			else return Error_TagUnclosed;
		}

		auto pos_BegBracket = SeekForChar(pos, end, '<');
		// Range checked: $pos_BegBracket will at least at $end.
		auto pos_EndBracket = SeekForEndBracket(pos_BegBracket, end);
		// Append new content.
		if (pos_BegBracket == end) // Unable to find '<'.
		{
			// Error: No more tags available, but a unexpected '>' present.
			if (pos_EndBracket != end) return Error_UnexpectedChar;
			// Finish: If root element has been closed, the deserialization is finished successfully.
			if (elemStack.empty()) return err;
			// Error: Root element has not been closed, but there is no more tag available.
			else return Error_EndTagNotFound;
		}
		else
		{
			// Error: '<' was found but it lack a '>'.
			if (pos_EndBracket == end) return Error_TagUnclosed;
			if (!elemStack.empty())
				elemStack.top()->AppendContent({ pos_LastEndBracket, pos_BegBracket });
		}

		// Now there should be a well-formed tag between $pos and $end.
		// It is possible that $pos_EndName is at $end.

		err = _Doc._ParseTag(elemStack, pos_BegBracket, pos_EndBracket);
		// Error: Internal error occurred.
		if (!err.IsSuccess()) return err;

		pos_LastEndBracket = pos_EndBracket + 1;
		pos = pos_EndBracket + 1;
	}
	// Finish: No error occurred yet.
	return Success_Perfect;
}

_L_NS_END_SERIALIZATION
