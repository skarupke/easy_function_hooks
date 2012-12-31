#pragma once

#include "meta.h"
#include <functional>

namespace fcf
{

template<typename T, typename P, P ptr>
struct unique_storer
{
	static T func;
};
template<typename T, typename P, P ptr>
T unique_storer<T, P, ptr>::func;

template<typename>
struct call_stored_function;
template<typename>
struct member_call_stored_function;

template<typename Result, typename... Args>
struct call_stored_function<std::function<Result (Args...)>>
{
	// this double nesting is necessary because of bugs in visual studio's
	// variadic templates. (dec 2012)
	// removing it should be trivial because we always know that
	// CallType == std::function<Result (Args...)>
	template<typename CallType>
	struct caller
	{
		template<CallType * func>
		static Result call(Args... args)
		{
			return (*func)(std::forward<Args>(args)...);
		}
	};
};

template<typename Result, typename Class, typename... Args>
struct member_call_stored_function<std::function<Result (Class &, Args...)>>
{
	// this double nesting is necessary because of bugs in visual studio's
	// variadic templates. (dec 2012)
	// removing it should be trivial because we always know that
	// CallType == std::function<Result (Class &, Args...)>
	template<typename CallType>
	struct caller
	{
		template<CallType * func>
		Result call(Args... args)
		{
			return (*func)(reinterpret_cast<Class &>(*this), std::forward<Args>(args)...);
		}
	};
};
template<typename Result, typename Class, typename... Args>
struct member_call_stored_function<std::function<Result (const Class &, Args...)>>
{
	// this double nesting is necessary because of bugs in visual studio's
	// variadic templates. (dec 2012)
	// removing it should be trivial because we always know that
	// CallType == std::function<Result (const Class &, Args...)>
	template<typename CallType>
	struct caller
	{
		template<CallType * func>
		Result call(Args... args) const
		{
			return (*func)(reinterpret_cast<const Class &>(*this), std::forward<Args>(args)...);
		}
	};
};

}
