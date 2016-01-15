// File: Lazy.hpp
// Author: Rendong Liang (Liong)

#ifndef _L_Lazy
#define _L_Lazy
#include "Fundamental.hpp"

namespace LiongPlus
{
    enum class LazyThreadSafetyMode
    {
        ExecutionAndPublication,
        None,
        PublicationOnly
    }

    template<typename T>
    class Lazy<T>
    {
        class LazyThreadSafeCaller
        {
        public:
            virtual GetValue() = 0;
        }
        
        class LazyThreadSafeSharedCaller
            : public LazyThreadSafeCaller
        {
        public:
            virtual GetValue() override final
            {
                _Mutex.lock();
                
                _Mutex.unlock();
            }
        private:
            std::mutex _Mutex;
        }
        
        class LazyThreadSafeOccupiedCaller
            : public LazyThreadSafeCaller
        {
        public:
            virtual Getvalue() override final
            {
                if (std::this_thread->thread_id)
                    ;
                else
                    throw Thread
            }
        private:
            int threadId;
        }        
        
    public:
        Lazy()
            : _Initializer(new Func<T(void)>([]{ return new T; }))
            , _Value(nullptr)
        {
        }
        Lazy(const Lazy<T>& instance) = delete;
        Lazy(Lazy<T>&& instance)
            : _Initializer(nullptr)
            , _Value(nullptr)
        {
            Swap(_Initializer, instance._Initializer);
            Swap(_Value, instance._Value);
        }
        Lazy(LazyThreadSafetyMode mode)
            : _Initializer(new Func<T(void)>([]{ return new T; }))
            , _Value(nullptr)
        {
        }
        Lazy(Func<T>& initializer)
            : _Initializer(new Func<T(void)>(initializer))
            , _Value(nullptr)
        {
        }
        Lazy(Func<T>& initializer, LazyThreadSafetyMode mode)
            : _Initializer(new Func<T(void)>(initializer))
            , _Value(nullptr)
        {
        }
        
        Lazy<T>& operator=(const Lazy<T>& instance) = delete;
        Lazy<T>& operator=(Lazy<T>&& instance)
        {
            Swap(_Initializer, instance._Initializer);
            Swap(_Value, instance._Value);
        }
        
        T& GetValue()
        {
            if (_Value == nullptr)
                _Value = 
            return *_Value; 
        }
        
        bool IsValueCreated()
        {
            return _Value != nullptr;
        }
        
    private:
        Func<T(void)>* _Initializer;
        Ptr<T> _Value;
        std::mutex _Mutex;
    }
}

#endif
