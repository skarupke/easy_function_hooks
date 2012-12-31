#pragma once

#include <functional>
#include <type_traits>

namespace fcf
{

namespace meta
{
	template<typename T>
	struct remove_ref_cv
	{
		typedef typename std::remove_reference<typename std::remove_cv<T>::type>::type type;
	};
	
	template<typename>
	struct func_type;
	template<typename R, typename... Args>
	struct func_type<R (*)(Args...)> { typedef std::function<R (Args...)> type; };
	template<typename R, typename C, typename... Args>
	struct func_type<R (C::*)(Args...)> { typedef std::function<R (C &, Args...)> type; };
	template<typename R, typename C, typename... Args>
	struct func_type<R (C::*)(Args...) const> { typedef std::function<R (const C &, Args...)> type; };
	
	template<typename>
	struct class_type { typedef void type; };
	template<typename R, typename C, typename... Args>
	struct class_type<R (C::*)(Args...)> { typedef C type; };
	template<typename R, typename C, typename... Args>
	struct class_type<R (C::*)(Args...) const> { typedef C type; };

	template<typename, typename>
	struct is_method_compatible { static const bool value = false; };
	template<typename R, typename C1, typename C2, typename... Args>
	struct is_method_compatible<R (C1::*)(Args...), R (C2::*)(Args...)> { static const bool value = true; };
	template<typename R, typename C1, typename C2, typename... Args>
	struct is_method_compatible<R (C1::*)(Args...) const, R (C2::*)(Args...) const> { static const bool value = true; };

	template<typename T>
	struct is_method { static const bool value = !std::is_same<typename class_type<T>::type, void>::value; };
}

}
