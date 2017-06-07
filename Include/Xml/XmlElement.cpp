// File: XmlElement.cpp
// Author: Rendong Liang (Liong)
#include "XmlElement.hpp"

_L_NS_BEG_XML

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

string XmlElement::Content() { return _Content ?_Content->GetContent() : ""; }

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

_L_NS_END_XML
