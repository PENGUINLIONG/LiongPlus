// File: Delegate.hpp
// Author: Rendong Liang (Liong)

#ifndef _L_Delegate
#define _L_Delegate
#include "Fundamental.hpp"
#include "Exception.hpp"
#include "IDisposable.hpp"
#include "Ptr.hpp"

namespace LiongPlus
{
	template<typename TX>
	class Delegate
	{
	};

	/// <summary>
	/// A data structure that refers to a static method, to a lambda expression or to a class instance and an instance method of that class.
	/// </summary>
	/// <remarks>If you want to define a delegate with no argument, just leave it blank. Pass [void] as a template parameter is unavailable and may cause compile-time error.</remarks>
	/// <typeparam name="TResult">Type of return value.</typeparam>
	/// <typeparam name="TArgs">Type(s) argument(s).</typeparam>
	template<typename TResult, typename ... TArgs>
	class Delegate<TResult(TArgs ...)>
		: public Object
		, public IDisposable
	{
	private:
		typedef Delegate<TResult(TArgs ...)> TSelf;

		enum class InvokerType
		{
			Static,
			Member,
			Lambda
		};

		class Invoker
		{
		public:
			InvokerType _Type;

			Invoker(InvokerType type)
				: _Type(type)
			{
			}
			virtual ~Invoker()
			{
			}

			virtual Ptr<Invoker> Clone() = 0;
			virtual TResult Invoke(TArgs&& ...) = 0;
			virtual void* Method() = 0;
			virtual void* Target() = 0;
		};

		class StaticInvoker
			: public Invoker
		{
		private:
			typedef TResult(*TFunc)(TArgs ...);

			TFunc _Func;
		public:
			StaticInvoker(TFunc func)
				: _Func(func)
				, Invoker(InvokerType::Static)
			{
			}
			virtual ~StaticInvoker() override final
			{
			}

			virtual Ptr<Invoker> Clone() override final
			{
				return new StaticInvoker(_Func);
			}

			virtual TResult Invoke(TArgs&& ... args) override final
			{
				return _Func(Forward<TArgs ...>(args) ...);
			}

			virtual void* Method() override final
			{
				return _Func;
			}

			virtual void* Target() override final
			{
				return nullptr;
			}
		};

		template<typename TClass>
		class MemberInvoker
			: public Invoker
		{
		private:
			typedef TResult(TClass::*TFunc)(TArgs ...);

			TClass* _Class;
			TFunc _Func;
		public:
			MemberInvoker(TClass* owner, TFunc func)
				: _Class(owner)
				, _Func(func)
				, Invoker(InvokerType::Member)
			{
			}
			virtual ~MemberInvoker() override final
			{
			}

			virtual Ptr<Invoker> Clone() override final
			{
				return new MemberInvoker<TClass>(_Class, _Func);
			}

			virtual TResult Invoke(TArgs&& ... args) override final
			{
				return (_Class->*_Func)(Forward<TArgs ...>(args) ...);
			}

			virtual void* Method() override final
			{
				return nullptr;
			}

			virtual void* Target() override final
			{
				return _Class;
			}
		};

		template<typename TLambda>
		class LambdaInvoker
			: public Invoker
		{
		private:
			TLambda _Lambda; // Keep an instance to prevent null references.
		public:
			LambdaInvoker(const TLambda& lambda)
				: _Lambda(lambda)
				, Invoker(InvokerType::Lambda)
			{
			}
			virtual ~LambdaInvoker() override final
			{
			}

			virtual Ptr<Invoker> Clone() override final
			{
				return new LambdaInvoker<TLambda>(_Lambda);
			}

			virtual TResult Invoke(TArgs&& ... args) override final
			{
				return _Lambda.operator()(Forward<TArgs ...>(args) ...);
			}

			virtual void* Method() override final
			{
				return nullptr;
			}

			virtual void* Target() override final
			{
				return &_Lambda;
			}
		};

		Ptr<Invoker> _Invoker;

		virtual void Dispose() override final
		{
			if (_Invoker)
				_Invoker = nullptr;
		}
	public:
		template<typename TClass>
		struct ClassFuncTuple;

		template<typename TClass>
		struct ClassFuncTuple<TClass*>
		{
		public:
			TClass* _Owner;
			const TResult(TClass::*_Func)(TArgs ...);

			ClassFuncTuple(TClass* owner, const TResult(TClass::*func)(TArgs ...))
				: _Owner(owner)
				, _Func(func)
			{
			}
			~ClassFuncTuple()
			{
			}
		};

		template<typename TClass>
		struct ClassFuncTuple<TClass&>
		{
		public:
			TClass* _Owner;
			const TResult(TClass::*_Func)(TArgs ...);

			ClassFuncTuple(TClass& owner, const TResult(TClass::*func)(TArgs ...))
				: _Owner(&owner)
				, _Func(func)
			{
			}
			~ClassFuncTuple()
			{
			}
		};

		template<typename TClass>
		struct ClassFuncTuple<TClass&&>
		{
		public:
			TClass* _Owner;
			const TResult(TClass::*_Func)(TArgs ...);

			ClassFuncTuple(TClass&& owner, const TResult(TClass::*func)(TArgs ...))
			{
				throw ArgumentException("Need non-temporary instance.");
			}
			~ClassFuncTuple()
			{
			}
		};

		Delegate()
			: _Invoker(nullptr)
		{
		}
		Delegate(const nullptr_t null)
			: _Invoker(nullptr)
		{
		}
		Delegate(const TSelf& instance)
			: _Invoker(instance._Invoker->Clone())
		{
		}
		Delegate(TResult(*func)(TArgs ...))
			: _Invoker(new StaticInvoker(func))
		{
		}
		template<typename TClass>
		Delegate(TClass* owner, TResult(TClass::*func)(TArgs ...))
			: _Invoker(new MemberInvoker<TClass>(owner, func))
		{
		}
		template<typename TClass>
		Delegate(TClass& owner, TResult(TClass::*func)(TArgs ...))
			: _Invoker(new MemberInvoker<TClass>(&owner, func))
		{
		}
		template<typename TClass>
		Delegate(TClass&& owner, TResult(TClass::*func)(TArgs ...))
		{
			throw ArgumentException("Need non-temporary instance.");
		}
		template<typename TLambda>
		Delegate(const TLambda& lambda)
			: _Invoker(new LambdaInvoker<TLambda>(lambda))
		{
		}
		~Delegate()
		{
			Dispose();
		}

		TSelf& operator=(nullptr_t null)
		{
			Dispose();
			return *this;
		}
		TSelf& operator=(const TSelf& instance)
		{
			Dispose();
			_Invoker = instance._Invoker->Clone();
			return *this;
		}
		TSelf& operator=(TResult(*func)(TArgs ...))
		{
			Dispose();
			_Invoker = new StaticInvoker(func);
			return *this;
		}
		template<typename TClass>
		TSelf& operator=(ClassFuncTuple<TClass> tuple)
		{
			Dispose();
			_Invoker = new MemberInvoker<TClass>(tuple._Owner, tuple._Func);
			return *this;
		}
		template<typename TLambda>
		TSelf& operator=(const TLambda& lambda)
		{
			Dispose();
			_Invoker = new LambdaInvoker<TLambda>(lambda);
			return *this;
		}

		bool operator==(const nullptr_t null) const
		{
			return _Invoker;
		}
		bool operator==(const TSelf& instance) const
		{
			return Equals(instance);
		}
		bool operator!=(const nullptr_t null) const
		{
			return !_Invoker;
		}
		bool operator!=(const TSelf& instance) const
		{
			return !Equals(instance);
		}
		bool operator!() const
		{
			return !_Invoker;
		}

		operator bool() const
		{
			return _Invoker != nullptr;
		}

		TResult operator()(TArgs ... args)
		{
			if (_Invoker)
				return _Invoker->Invoke(Forward<TArgs>(args) ...);
			else
				throw TargetInvocationException("Invalid invoker.");
		}

		bool Equals(const TSelf& obj) const
		{
			if (_Invoker)
			{
				if (obj._Invoker)
					return _Invoker->Method() == obj._Invoker->Method() && _Invoker->Target() == obj._Invoker->Target();
				else
					return false;
			}
			else
			{
				if (obj._Invoker)
					return false;
				else
					return true;
			}
		}
	};

	// This is an example of delegate announcement with special syntax of LiongPlus.
#ifdef _L_SYNTAX
	template<typename ... Ts>
	delegate(Action) < void(Ts ...) >;
#else
	template<typename ... Ts>
	using Action = Delegate<void(Ts ...)>;
#endif
	template<typename TResult, typename ... TArgs>
	using Func = Delegate<TResult(TArgs ...)>;

	template<typename T>
	using Predicate = Delegate<bool(T)>;

	template<typename TInput, typename TOutput>
	using Converter = Delegate<TOutput(const TInput)>;

	template<typename T>
	using Comparison = Delegate<int(T, T)>;
}
#endif