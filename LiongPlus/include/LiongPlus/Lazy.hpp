// File: Lazy.hpp
// Author: Rendong Liang (Liong)

#ifndef _L_Lazy
#define _L_Lazy
#include "Fundamental.hpp"
#include <mutex>
#include <thread>

namespace LiongPlus
{
	enum class LazyThreadSafetyMode
	{
		ExecutionAndPublication,
		None,
		PublicationOnly
	};

	template<typename T>
	class Lazy
	{
	private:
		using TInitializer = Func<T*(void)>;

		class LazyObjectCaller
		{
		public:
			LazyObjectCaller(TInitializer& initializer)
				: _Initializer(new TInitializer(initializer))
				: _Value(nullptr)
			{
			}
			virtual ~LazyThreadSafeCaller()
			{
				if (_Initializer != nullptr)
				{
					delete _Initializer;
					_Initializer = nullptr;
				}
			}

			bool IsValueCreated()
			{
				return _Value != nullptr;
			}
			virtual T& GetValue() = 0;
		private:
			TInitializer* _Initializer;
			Ptr<T> _Value;
		};

		class LazyThreadUnsafeCaller
			: public LazyObjectCaller
		{
		public:
			LazyThreadUnsafeCaller(TInitializer& initializer)
				: LazyObjectCaller(initializer)
			{
			}
			virtual ~LazyThreadUnsafeCaller()
			{
			}

			virtual T& GetValue() override final
			{
				if (_Value == nullptr)
				{
					_Value = (*_Initializer)();
					delete _Initializer;
					_Initializer = nullptr;
				}

				return *_Value;
			}
		};
		
		class LazyThreadSafeSharedCaller
			: public LazyObjectCaller
		{
		public:
			LazyThreadSafeSharedCaller(TInitializer& initializer)
				: LazyObjectCaller(initializer)
			{
			}
			virtual ~LazyThreadSafeSharedCaller()
			{
			}

			virtual T& GetValue() override final
			{
				_Mutex.lock();
				if (_Value == nullptr)
				{
					_Value = (*_Initializer)();
					delete _Initializer;
					_Initializer = nullptr;
				}
				_Mutex.unlock();

				return *_Value;
			}
		private:
			std::mutex _Mutex;
		};
		
		class LazyThreadSafeOccupiedCaller
			: public LazyObjectCaller
		{
		public:
			LazyThreadSafeOccupiedCaller(TInitializer& initializer)
				: LazyObjectCaller(initializer)
				: _ThreadId(0)
			{
			}
			virtual ~LazyThreadSafeOccupiedCaller()
			{
				_ThreadId = 0;
			}

			virtual T& GetValue() override final
			{
				_Mutex.lock();
				if (_ThreadId == 0)
				{
					auto tid = std::this_thread::get_id();
					_ThreadId = *((unsigned long*)(&tid));
				}
				if (this_thread::get_id() == _ThreadId)
				{
					if (_Value == nullptr)
					{
						_Value = (*_Initializer)();
						delete _Initializer;
						_Initializer = nullptr;
					}
				}
				else throw InvalidOperationException("Current thread failed to fetch the permission from the lazy object.");
				_Mutex.unlock();

				return *_Value;
			}
		private:
			unsigned long _ThreadId;
			std::mutex _Mutex;
		};
		
	public:
		Lazy()
			: _Value(new LazyThreadSafeSharedCaller(new Func<T*(void)>([] { return new T; })))
		{
		}
		Lazy(const Lazy<T>& instance) = delete;
		Lazy(Lazy<T>&& instance)
			: _Value(nullptr)
		{
			swap(_Value, instance._Value);
		}
		Lazy(LazyThreadSafetyMode mode)
		{
			switch (mode)
			{
				case LazyThreadSafetyMode::ExecutionAndPublication:
					_Value(new LazyThreadSafeSharedCaller(new Func<T*(void)>([] { return new T; })));
					break;
				case LazyThreadSafetyMode::None:
					_Value(new LazyThreadUnsafeCaller(new Func<T*(void)>([] { return new T; })));
					break;
				case LazyThreadSafetyMode::PublicationOnly:
					_Value(new LazyThreadSafeOccupiedCaller(new Func<T*(void)>([] { return new T; })));
					break;
				default:
					throw std::runtime_error("Invalid $mode.");
			}
		}
		Lazy(Func<T>& initializer)
			: _Value(new LazyThreadSafeSharedCaller(initializer))
		{
		}
		Lazy(Func<T>& initializer, LazyThreadSafetyMode mode)
		{
			switch (mode)
			{
				case LazyThreadSafetyMode::ExecutionAndPublication:
					_Value(new LazyThreadSafeSharedCaller(initializer));
					break;
				case LazyThreadSafetyMode::None:
					_Value(new LazyThreadUnsafeCaller(initializer));
					break;
				case LazyThreadSafetyMode::PublicationOnly:
					_Value(new LazyThreadSafeOccupiedCaller(initializer));
					break;
				default:
					throw std::runtime_error("Invalid $mode.");
			}
		}
		~Lazy()
		{
			if (_Value != nullptr)
			{
				delete _Value;
				_Value = nullptr;
			}
		}
		
		Lazy<T>& operator=(const Lazy<T>& instance) = delete;
		Lazy<T>& operator=(Lazy<T>&& instance)
		{
			swap(_Value, instance._Value);
		}
		
		T& GetValue()
		{
			return _Value->GetValue();
		}
		
		bool IsValueCreated()
		{
			return _Value->IsValueCreated();
		}
		
	private:
		unique_ptr<LazyObjectCaller> _Value;
	};
}
#endif
