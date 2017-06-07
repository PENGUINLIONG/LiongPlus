#pragma once
#include "_"
#include <memory>
#include <type_traits>
#include <typeinfo>
#include <utility>

_L_NS_BEG

namespace details
{
	template<typename T, typename U>
	constexpr bool is_convertible()
	{
		return std::is_same_v<T, U> || std::is_base_of_v<U, T> || std::is_base_of_v<T, U>;
	}
}

// It does not offer run-time acrobatics.
template<typename T>
struct Interface
{
	static_assert(std::is_compound<T>::value, "T should be a compound type.");
protected:
	std::shared_ptr<T> _Ref;
	
public:
	Interface() = default;
	Interface(const Interface<T>&) = default;
	Interface(Interface&&) = default;
	template<typename U>
	Interface(const Interface<U>& instance) noexcept :
		_Ref(std::static_pointer_cast<T>(instance.Get()))
	{
		static_assert(details::is_convertible<T, U>(), "T should be convertible to U.");
	}
	template<typename U>
	Interface(const std::weak_ptr<U>& ptr) noexcept :
		_Ref(std::static_pointer_cast<T>(ptr.lock()))
	{
		static_assert(details::is_convertible<T, U>(), "T should be convertible to U.");
	}
	template<typename U>
	Interface(const std::shared_ptr<U>& ptr) noexcept :
		_Ref(std::static_pointer_cast<T>(ptr))
	{
		static_assert(details::is_convertible<T, U>(), "T should be convertible to U.");
	}
	Interface(const nullptr_t&) noexcept :
		_Ref(nullptr)
	{}
	~Interface() noexcept
	{
	}

	Interface<T>& operator=(const Interface<T>&) = default;
	Interface<T>& operator=(Interface<T>&&) = default;

	bool operator==(const Interface<T>& other) const
	{
		return _Ref == other._Ref;
	}
	bool operator<(const Interface<T>& other) const
	{
		return _Ref < other._Ref;
	}

	template<typename U>
	operator std::shared_ptr<U>() const
	{
		if (!CanCastTo<U>()) throw std::logic_error((std::string("Cannot cast from ") + typeid(T).name() + " to type" + typeid(U).name() + ".").c_str());
		return std::static_pointer_cast<U>(_Ref);
	}
	template<typename U>
	operator std::weak_ptr<U>() const
	{
		return static_cast<std::shared_ptr<U>>(*this);
	}

	bool IsValid() const noexcept
	{
		return _Ref != nullptr;
	}

	/*
	 * This check simply check if it is convertible to U from T rather than RTTI for performance.
	 * It does NOT guarantee that the type cast will be done correctly.
	 */
	template<typename U>
	bool CanCastTo() const noexcept
	{
		return details::is_convertible<T, U>();
	}

	T& operator*() const noexcept
	{
		return Get.operator*();
	}
	T* operator->() const noexcept
	{
		return Get().operator->();
	}
	
	template<typename U = T>
	std::shared_ptr<U> Get() const noexcept
	{
		return std::static_pointer_cast<U>(_Ref);
	}
	template<>
	std::shared_ptr<T> Get<T>() const noexcept
	{
		return _Ref;
	}
};

template<typename T>
using I = Interface<T>;

_L_NS_END
