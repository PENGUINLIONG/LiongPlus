#include <functional>
#include "_"

// This is an example of delegate announcement with special syntax of LiongPlus.
#ifdef _L_SYNTAX
template<typename ... TArgs>
delegate(Action) < void(TArgs ...) >;
#else
template<typename ... TArgs>
using Action = std::function<void(TArgs ...)>;
#endif
template<typename TResult, typename ... TArgs>
using Func = std::function<TResult(TArgs ...)>;

template<typename T>
using Predicate = std::function<bool(T)>;

template<typename TInput, typename TOutput>
using Converter = std::function<TOutput(const TInput)>;

template<typename T>
using Comparison = std::function<int(T, T)>;