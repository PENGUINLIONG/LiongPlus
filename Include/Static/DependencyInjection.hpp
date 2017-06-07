#pragma once
#include "_"
#include <tuple>
#include <type_traits>
#include <cstdint>
#include <string>

/*
 * Dependency Injection
 *
 * This static system is made for separating different components from the framework,
 * in order to reduce coupling.
 *
 * The major components of this system are the three:
 *
 * - DependencyProperty: Describing the types of dependency properties.
 *   It is also responsible for the initialization of properties
 * - Dependency: Describing the order of a pack of dependency properties.
 * - DependencyContext: allocating and freeing of dependency properties.
 *   It also provide name-based property retrieval.
 *
 * This system is designed to do what it should do,
 * while taking the minimum run-time costs and lines of codes.
 * To exploit the system, follow the instructions below.
 *
 * 1. Write down the dependency properties (NAME CANNOT BE CHANGED):
 * ```
 * using dependency = Dependency <
 *     DependencyProperty<array<unsigned char, "Buffer"_dep>>,
 *     DependencyProperty<size_t, "Width"_dep>,
 *     DependencyProperty<size_t, "Height"_dep>
 * >;
 * dependency _DepObj;
 * ```
 *    Notice that you can also customize the initializer of properties.
 *    By default, properties are default-constructed.
 * ```
 * struct CustomizedSizeInitializer
 * {
 * public:
 *     static constexpr size_t Init()
 *     {
 *         return 1024;
 *     }
 * };
 * ...
 * DependencyProperty<size_t, "Width"_dep, CustomizedSizeInitializer>;
 * ```
 * 2. Make the access tokens (Optional):
 * ```
 * enum
 * {
 *     BufferProperty,
 *     WidthProperty,
 *     HeightProperty
 * }
 * ```
 * 4. Get your dependency object:
 * ```
 * d.dependency.Get<dep::WidthProperty>();
 * DependencyContext::GetByName<size_t>(d, "Height") << endl;
 * ```
 */

_L_NS_BEG

// Dependency injection default initializer
template<typename T>
struct _L_DIDI
{
public:
	static constexpr T Init()
	{
		return{};
	}
};

constexpr uint64_t depHash(const char* str)
{
#pragma warning (push)
#pragma warning (disable: 4307)
	// FVN-1 algorithm.
	return (*str == '\0' ? 0xcbf29ce484222325Ui64 : (0x100000001b3Ui64 * depHash(str + 1)) ^ static_cast<uint64_t>(*str)) & 0xFFFFFFFFFFFFFFFFUi64;
#pragma warning (pop)
}

constexpr uint64_t operator"" _dep(const char* str, size_t length)
{
	return depHash(str);
}

template<typename T, uint64_t THash, typename TInitializer = _L_DIDI<T>>
struct DependencyProperty
{
public:
	using value_type = T;

	static const uint64_t hashcode = THash;

	static constexpr T DefaultConstruct()
	{
		return TInitializer::Init();
	}

	static constexpr bool Validate(const T& value)
	{
		return TValidator::Validate(value);
	}
};

template<typename ... TProperties>
struct Dependency;

namespace detail
{
	template<typename T>
	struct Count;
	template<typename TFirst, typename ... TOthers>
	struct Count<Dependency<TFirst, TOthers ...>>
	{
		static constexpr size_t count = Count<Dependency<TOthers ...>>::count + 1;
	};
	template<>
	struct Count<Dependency<>>
	{
	public:
		static constexpr size_t count = 0;
	};

	template<size_t TIndex, typename TDepObj>
	struct PropertyAt;
	template<size_t TIndex, typename TFirst, typename ... TProperties>
	struct PropertyAt<TIndex, Dependency<TFirst, TProperties...>>
	{
		using value_type = typename PropertyAt<TIndex - 1, Dependency<TProperties...>>::value_type;
		using property_type = typename PropertyAt<TIndex - 1, Dependency<TProperties...>>::property_type;
	};
	template<typename TFirst, typename ... TProperties>
	struct PropertyAt<0, Dependency<TFirst, TProperties...>>
	{
		using value_type = typename TFirst::value_type;
		using property_type = typename TFirst;
	};

	template<typename T>
	struct IsDependencyObject
	{
	public:
		static constexpr bool value = false;
	};
	template<typename ... TProperties>
	struct IsDependencyObject<Dependency<TProperties...>>
	{
	public:
		static constexpr bool value = true;
	};
}

template<typename ... TProperties>
struct Dependency
{
private:
	using self = Dependency<TProperties ...>;

public:
	using properties_tuple_type = std::tuple<typename TProperties::value_type ...>;

	properties_tuple_type& properties;

	Dependency()
		: properties(DependencyContext::Allocate<self>())
	{
	}
	Dependency(const self& inst)
		: properties(DependencyContext::AllocateAndCopyConstruct<self>())
	{
	}
	~Dependency()
	{
		DependencyContext::Free<self>(properties);
	}

	self operator=(const self& inst)
	{
		DependencyContext::Free(inst);
		DependencyContext::AllocateAndCopyConstruct(inst);
	}

	template<size_t TIndex>
	typename detail::PropertyAt<TIndex, Dependency<TProperties ...>>::value_type& Get()
	{
		return std::get<TIndex>(properties);
	}

};

struct DependencyContext
{
	template<typename ... TProperties>
	friend struct Dependency;
private:
	template<typename T, size_t ... TIndices>
	static typename T::properties_tuple_type& AllocateImpl(std::index_sequence<TIndices ...> sequence)
	{
		return *(new T::properties_tuple_type{ detail::PropertyAt<TIndices, T>::property_type::DefaultConstruct() ... });
	}
	template<typename T>
	static typename T::properties_tuple_type& Allocate()
	{
		static_assert(detail::IsDependencyObject<T>::value, "T should be a Dependency.");

		return AllocateImpl<T>(std::make_index_sequence<detail::Count<T>::count>());
	}

	template<typename T>
	static typename T::properties_tuple_type& AllocateAndCopyConstruct(const typename T::properties_tuple_type& copyFrom)
	{
		static_assert(detail::IsDependencyObject<T>::value, "T should be a Dependency.");

		return *(new T::properties_tuple_type{ copyFrom });
	}

	template<typename T>
	static void Free(typename T::properties_tuple_type& pack)
	{
		delete &pack;
	}

	template<typename TTarget, typename T, size_t ... TIndices>
	struct GetByNameImplImpl;
	template<typename TTarget, typename T>
	struct GetByNameImplImpl<TTarget, T>
	{
		static TTarget& Do(const T& inst, const uint64_t hashcode, std::index_sequence<> sequence)
		{
			throw std::runtime_error("Unable to find dependency property!");
		}
	};
	template<typename TTarget, typename T, size_t TIndex, size_t ... TOthers>
	struct GetByNameImplImpl<TTarget, T, TIndex, TOthers ...>
	{
		static TTarget& Do(const T& inst, const uint64_t hashcode, std::index_sequence<TIndex, TOthers ...> sequence)
		{
			if (detail::PropertyAt<TIndex, T::dependency_type>::property_type::hashcode == hashcode)
				return reinterpret_cast<TTarget&>(std::get<TIndex>(reinterpret_cast<typename T::dependency_type*>(inst.dependency)->properties));
			else
				return GetByNameImplImpl<TTarget, T, TOthers...>::Do(inst, hashcode, index_sequence<TOthers...>());
		}
	};

	template<typename TTarget, typename T, size_t ... TIndices>
	static typename TTarget& GetByNameImpl(const T& inst, const uint64_t hashcode, std::index_sequence<TIndices ...> sequence)
	{
		return GetByNameImplImpl<TTarget, T, TIndices ...>::Do(inst, hashcode, sequence);
	}



	template<typename TTarget, typename T, size_t ... TIndices>
	struct SetByNameImplImpl;
	template<typename TTarget, typename T>
	struct SetByNameImplImpl<TTarget, T>
	{
		static std::tuple<size_t, TTarget&> Do(const T& inst, const uint64_t hashcode, const TTarget& value, std::index_sequence<> sequence)
		{
			throw std::runtime_error("Unable to find dependency property!");
		}
	};
	template<typename TTarget, typename T, size_t TIndex, size_t ... TOthers>
	struct SetByNameImplImpl<TTarget, T, TIndex, TOthers ...>
	{
		static TTarget& Do(const T& inst, const uint64_t hashcode, TTarget&& value, std::index_sequence<TIndex, TOthers ...> sequence)
		{
			if (detail::PropertyAt<TIndex, T::dependency_type>::property_type::hashcode == hashcode)
				reinterpret_cast<TTarget&>(std::get<TIndex>(reinterpret_cast<typename T::dependency_type*>(inst.dependency)->properties)) = std::forward<TTarget>(value);
			else
				return SetByNameImplImpl<TTarget, T, TOthers...>::Do(inst, hashcode, std::forward<TTarget>(value), index_sequence<TOthers...>());
		}
		static TTarget& Do(const T& inst, const uint64_t hashcode, const TTarget& value, std::index_sequence<TIndex, TOthers ...> sequence)
		{
			if (detail::PropertyAt<TIndex, T::dependency_type>::property_type::hashcode == hashcode)
				reinterpret_cast<TTarget&>(std::get<TIndex>(reinterpret_cast<typename T::dependency_type*>(inst.dependency)->properties)) = value;
			else
				return SetByNameImplImpl<TTarget, T, TOthers...>::Do(inst, hashcode, value, index_sequence<TOthers...>());
		}
	};

	template<typename TTarget, typename T, size_t ... TIndices>
	static typename TTarget& SetByNameImpl(const T& inst, const uint64_t hashcode, TTarget&& value, std::index_sequence<TIndices ...> sequence)
	{
		return SetByNameImplImpl<TTarget, T, TIndices ...>::Do(inst, hashcode, std::forward<TTarget>(value), sequence);
	}
	template<typename TTarget, typename T, size_t ... TIndices>
	static typename TTarget& SetByNameImpl(const T& inst, const uint64_t hashcode, const TTarget& value, std::index_sequence<TIndices ...> sequence)
	{
		return SetByNameImplImpl<TTarget, T, TIndices ...>::Do(inst, hashcode, value, sequence);
	}

public:
	template<typename TTarget, typename T>
	static const TTarget& GetByName(const T& inst, const uint64_t hashcode)
	{
		return GetByNameImpl<TTarget>(inst, hashcode, std::make_index_sequence<detail::Count<T::dependency_type>::count>());
	}
	template<typename TTarget, typename T>
	static const TTarget& GetByName(const T& inst, const std::string& name)
	{
		return GetByName<TTarget, T>(inst, depHash(name.c_str()));
	}
	template<typename TTarget, typename T>
	static const TTarget& GetByName(const T& inst, const char* name)
	{
		return GetByName<TTarget, T>(inst, depHash(name));
	}

	template<typename TTarget, typename T>
	static void SetByName(const T& inst, const uint64_t hashcode, TTarget&& value)
	{
		return SetByNameImpl<TTarget>(inst, hashcode, std::forward<TTarget>(value), std::make_index_sequence<detail::Count<T::dependency_type>::count>());
	}
	template<typename TTarget, typename T>
	static void SetByName(const T& inst, const std::string& name, TTarget&& value)
	{
		return SetByName<TTarget, T>(inst, depHash(name.c_str()), std::forward<TTarget>(value));
	}
	template<typename TTarget, typename T>
	static void SetByName(const T& inst, const char* name, TTarget&& value)
	{
		return SetByName<TTarget, T>(inst, depHash(name), std::forward<TTarget>(value));
	}
	template<typename TTarget, typename T>
	static void SetByName(const T& inst, const uint64_t hashcode, const TTarget& value)
	{
		return SetByNameImpl<TTarget>(inst, hashcode, value, std::make_index_sequence<detail::Count<T::dependency_type>::count>());
	}
	template<typename TTarget, typename T>
	static void SetByName(const T& inst, const std::string& name, const TTarget& value)
	{
		return SetByName<TTarget, T>(inst, depHash(name.c_str()), value);
	}
	template<typename TTarget, typename T>
	static void SetByName(const T& inst, const char* name, const TTarget& value)
	{
		return SetByName<TTarget, T>(inst, depHash(name), value);
	}
};

template<typename TA, typename TB>
struct MergeDependencies;
template<typename ... TAProps, typename ... TBProps>
struct MergeDependencies<Dependency<TAProps ...>, Dependency<TBProps ...>>
{
	using result_type = Dependency<TAProps ..., TBProps ...>;
};

class DependencyObject
{
public:
	void* dependency;

	virtual void _AllocateDependency() = 0;
	virtual void _FreeDependency() = 0;
};

#define _L_OVERRIDE_DEPENDENCY_OBJECT_METHODS \
virtual void _AllocateDependency() override { dependency = new dependency_type; } \
virtual void _FreeDependency() override { delete reinterpret_cast<dependency_type*>(dependency); } \
dependency_type& GetDependencyRef() \
{ return *reinterpret_cast<dependency_type*>(dependency); }

template<typename T, typename ... TArgs>
std::shared_ptr<T> make_dep_obj(TArgs&& ... args)
{
	auto ptr = std::shared_ptr<T>(new T(std::forward<TArgs>(args) ...), [=](T* ptr) { ptr->_FreeDependency(); delete ptr; });
	ptr->_AllocateDependency();
	return ptr;
}

#define _L_CT_GET_DEPENDENCY(pos) \
(reinterpret_cast<dependency_type*>(dependency))->Get<pos>()

_L_NS_END