// File: DependencyInjection.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include <functional>
#include <map>
#include <memory>
#include <string>
#include "_"
#include "../../Threading/Dispatcher.hpp"
#include "UIUpdater.UIAction.hpp"

_L_NS_BEG_GRAPHICS_UI

/*
 * enum UIActionTrigger
 *
 * Enumeration of designed action set allowing the change of dependency properties to trigger
 *   re-configuration of UI status.
 *
 * Enumerations:
 *   None: Nothing happens on value change. Indicating the UI won't be affected by the change of
 *     dependency property.
 *   Render: Re-render on value change.
 *   Layout: Rearrange layout on value change.
 *   LogicTree: Re-establish logic relationship among UI elements.
 */
enum class UIActionTrigger : long
{
	None =                    0b0000'0000,
	Render =                  0b0000'0001,
	Layout =                  0b0000'0010,
	LogicTree =               0b0000'0100,

	Layout_Render =           0b0000'0011,
	LogicTree_Layout_Render = 0b0000'0111,
};

struct UIProperty
{
	friend class DependencyHost;
private:

	template<typename T>
	static std::function<bool(const void*)> DefaultValidator()
	{
		return [](const void* raw) -> bool { return true; };
	}
	template<typename T>
	static std::function<bool(const void*)> ValidatorParser(
		std::function<bool(const T&)>& validator)
	{
		return [&validator](const void* raw) -> bool { return validator(*(static_cast<const T*>(raw))); };
	}

public:
	using internal_validator_t = std::function<bool(const void*)>;
	template<typename T>
	using validator_t = std::function<bool(const T&)>;
	using allocator_t = std::function<void*()>;
	using deleter_t = std::function<void(void*)>;

	size_t Hash;
	std::string Name;
	UIActionTrigger Trigger;
	internal_validator_t Validator;
	allocator_t Allocator;
	deleter_t Deleter;

	inline bool operator==(const UIProperty& other) const
	{
		return Hash == other.Hash;
	}
	inline bool operator<(const UIProperty& other) const
	{
		return Hash < other.Hash;
	}

	template<typename TContainer>
	const UIProperty& Register() const
	{
		details::getPropMap()[typeid(TContainer).name()].push_back(*this);
		return *this;
	}
	template<typename TContainer>
	const UIProperty& RegisterAttached() const
	{
		details::getAttachedPropMap()[typeid(TContainer).name()].push_back(*this);
		return *this;
	}

	template<typename T>
	static constexpr const UIProperty MakeDefault(
		std::string name, UIActionTrigger trigger = UIActionTrigger::None,
		validator_t<T> validator = nullptr)
	{
		return
		{
			std::hash<std::string>{}(name), name, trigger,
			nullptr == validator.target<void>() ?
			DefaultValidator<T>() : ValidatorParser(validator),
			[]() ->void* { return new T {}; },
			[](void* field) { delete static_cast<T*>(field); }
		};
	}
	template<typename T>
	static constexpr const UIProperty Make(
		std::string name, UIActionTrigger trigger,
		validator_t<T> validator, allocator_t allocator, deleter_t deleter)
	{
		return
		{
			std::hash<std::string>{}(name), name, trigger,
			nullptr == validator.target<void>() ?
			DefaultValidator<T>() : ValidatorParser(validator),
			allocator,
			deleter
		};
	}
};

namespace details
{
	using prop_list = std::vector<UIProperty>;
	using prop_set = std::map<std::string, prop_list>;
	// { type_name: { property, property, ... } }
	prop_set& getPropMap();
	prop_set& getAttachedPropMap();
}

class DependencyHost
{
	template<typename TContainer>
	friend class DependencyObject;
private:
	std::map<UIProperty, void*> _Props;

	/*
	 * Get pointer to inquired dependency property.
	 */
	void* _GetField(const UIProperty& prop) const
	{
		auto it = _Props.find(prop);
		return it == _Props.end() ? nullptr : it->second;
	}

	/*
	 * On initializing class instance, load all directly owned dependency properties.
	 */
	void _InitializeTypeProperties(const char* name)
	{
		for (const auto& prop : details::getPropMap().at(name))
			_Props.emplace(prop, prop.Allocator());
	}

public:
	/*
	* On finalizing class instance, remove all directly owned dependency properties *as well as
	*   attached ones*.
	*/
	~DependencyHost()
	{
		for (const auto& prop : _Props)
			prop.first.Deleter(prop.second);
	}

	template<typename T>
	void AttachProperty()
	{
		for (const auto& prop : details::getAttachedPropMap().at(typeid(T).name()))
			_Props.emplace(prop, prop.Allocator());
	}

	template<typename T, typename TUnref = std::remove_reference<T>::type>
	const TUnref& GetProperty(const UIProperty& prop) const
	{
		void* field;
		if (nullptr == (field = _GetField(prop)))
			throw std::runtime_error((std::string("Unable to find property") + prop.Name).c_str());
		return *static_cast<const TUnref*>(field);
	}

	template<typename T>
	bool SetProperty(const UIProperty& prop, const T& value)
	{
		if (prop.Validator(&value))
		{
			void* field;
			if (nullptr == (field = _GetField(prop)))
				throw std::runtime_error((std::string("Unable to find property") + prop.Name).c_str());
			*static_cast<std::remove_reference_t<T>*>(field) = value;

			if (long(prop.Trigger) & long(UIActionTrigger::Render))
				UIUpdater::_RequestRender();
			if (long(prop.Trigger) & long(UIActionTrigger::Layout))
				UIUpdater::_RequestLayout();
			if (long(prop.Trigger) & long(UIActionTrigger::LogicTree))
				UIUpdater::_RequestLogicTree();

			return true;
		}
		else return false;
	}

	template<typename T>
	bool SetProperty(const UIProperty& prop, T&& value)
	{
		if (prop.Validator(&value))
		{
			void* field;
			if (nullptr == (field = _GetField(prop)))
				throw std::runtime_error((std::string("Unable to find property") + prop.Name).c_str());
			*static_cast<std::remove_reference_t<T>*>(field) = std::forward<std::remove_reference_t<T>>(value);

			if (long(prop.Trigger) & long(UIActionTrigger::Render))
				UIUpdater::_RequestRender();
			if (long(prop.Trigger) & long(UIActionTrigger::Layout))
				UIUpdater::_RequestLayout();
			if (long(prop.Trigger) & long(UIActionTrigger::LogicTree))
				UIUpdater::_RequestLogicTree();

			return true;
		}
		else return false;
	}
};

template<typename TContainer>
class DependencyObject
{
public:
	DependencyObject()
	{
		static_cast<TContainer*>(this)->_InitializeTypeProperties(typeid(TContainer).name());
	}
};

_L_NS_END_GRAPHICS_UI
