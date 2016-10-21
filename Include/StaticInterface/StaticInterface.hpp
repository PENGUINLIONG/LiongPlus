// File: StaticInterface.hpp
// Author: Rendong Liang(Liong)
#pragma once
#include <type_traits>

/*
 * This file provided a extremely simple and efficient way to help you implement a static interface.
 * With static interface, you can reduce coupling and drop inefficient vtable for your code.
 *
 *
 * How To Use:
 *   1. Declare a interface class method:
 *     >> Method<_ReturnType_(_InterfaceName_::*)(_Args_ ...)>;
 *       -or-
 *     >> dm<_InterfaceName_, _ReturnType_, _Args_ ...>;
 *
 *   2. To parse a instance which implement the interface verbally:
 *     >> _InterfaceName_ i = make_interface<_InterfaceName_>(_Instance_, _MethodAddresses_ ...);
 *     You can also use auto to further simplify the statement.
 *
 *   3. To invoke methods through an static interface:
 *     >> i._MethodName_(_Args_ ...);
 *     And you will receive your return value.
 *   
 *   4. You can also invoke a method through a *verbal* interface directly:
 *     >> interface_invoke<_InterfaceName_>(_Instance_, _MethodAddresses_);
 *
 *
 * Memory Consumption Consideration:
 *   This implementation of static interface will store two pointers for each method:
 *
 *     1. The pointer to class instance.
 *     2. The pointer to class method.
 *
 *   It can somehow lead to trivial memory consumption. (But who cares) (Kappa)
 */

namespace LiongPlus
{
	namespace StaticInterface
	{
		namespace details
		{
			// We have got args!
			template<typename ... TArgs>
			struct IsVoidArg { typedef std::false_type type; };
			// There is NO arg!
			template<>
			struct IsVoidArg<> { typedef std::true_type type; };



			template<typename TIsVoidArg, typename TI, typename TR, typename ... TArgs>
			struct InterfaceInvokeImpl {};
			// Inoker for methods which have args.
			template<typename TI, typename TR, typename ... TArgs>
			struct InterfaceInvokeImpl<std::false_type, TI, TR, TArgs...>
			{
				static inline TR Invoke(TI& interface, TR(TI::*method_of_interface)(TArgs ...), TArgs ... args)
				{ return (interface.*method_of_interface)(std::forward<TArgs>(args) ...); }
			};
			// Inoker for methods which have NO args.
			template<typename TI, typename TR>
			struct InterfaceInvokeImpl<std::true_type, TI, TR>
			{
				static inline TR Invoke(TI& interface, TR(TI::*method_of_interface)())
				{ return (interface.*method_of_interface)(); }
			};



			template<typename TI, typename TMethod>
			struct RectifyMethodType {};
			template<typename TI, typename TR, typename TC, typename ... TArgs>
			struct RectifyMethodType<TI, TR(TC::*)(TArgs ...)>
			{ typedef TR(TI::*type)(TArgs ...); };
		}
		
		template<typename TI, typename TC, typename TR, typename ... TArgs, typename ... TActualArgs>
		inline TR interface_invoke(TC& instance, TR(TC::*method)(TArgs ...), TActualArgs ... args)
		{
			return details::InterfaceInvokeImpl<typename details::IsVoidArg<TArgs ...>::type, TI, TR, TArgs ...>::
				// Change the original class type to interface type.
				// This can help us hide class type information, while methodtion calls still work properly.
				Invoke(*((TI*)&instance), (TR(TI::*)(TArgs ...))method, (TArgs)args ...);
		}
		
		/*
		 * Note:
		 *   - Why [IsVoidArg] is used in [InterfaceInvokeImpl] but not here?
		 *   - Because the arguments are used in invoking methods.
		 *     In [InterfaceInvokeImpl], argument types are deduced from user input,
		 *     and a [void] type might be appended.
		 *     If you look closely at the templates here in [Method],
		 *     you may find out that the primal template is method pointer.
		 *     which means the argument types are directly fetched from the pointer type.
		 *     So if there is no argument types for the method,
		 *     there is no arg types for the templates.
		 */
		template<typename TFPtr>
		struct Method {};
		template<typename TR, typename TI, typename ... TArgs>
		struct Method<TR(TI::*)(TArgs...)>
		{
		private:
			TI& _Instance;
			TR(TI::*_MethodAddr)(TArgs ...);
		public:
			Method(TI& instance, TR(TI::*method)(TArgs ...)) : _Instance(instance), _MethodAddr(method) {}
			template<typename ... TActualArgs>
			TR operator()(TActualArgs&& ... args)
			{ return interface_invoke<TI>(_Instance, _MethodAddr, std::forward<TActualArgs>(args)...); }
		};
		template<typename TR, typename TI>
		struct Method<TR(TI::*)()>
		{
		private:
			TI& _Instance;
			TR(TI::*_MethodAddr)();
		public:
			Method(TI& instance, TR(TI::*method)()) : _Instance(instance), _MethodAddr(method) {}
			TR operator()()
			{ return interface_invoke<TI>(_Instance, _MethodAddr); }
		};
		
		template<typename TI, typename TC, typename ... TMethods>
		TI make_interface(TC& instance, TMethods ... methods)
		{
			return TI
			{
				Method<typename details::RectifyMethodType<TI, TMethods>::type>
				(*((TI*)&instance), (typename details::RectifyMethodType<TI, TMethods>::type)methods) ...
			};
		}
		
		// 'dm' stand for 'declare method'.
		template<typename TInterface, typename TReturn, typename ... TArgs>
		using dm = Method<TReturn(TInterface::*)(TArgs ...)>;
	}
}
