#pragma once

#include "meta.h"
#include <functional>
#include <vector>

namespace fcf
{
template<typename K>
std::vector<std::pair<K, typename meta::func_type<K>::type *>> & getUniqueStorers()
{
	static std::vector<std::pair<K, typename meta::func_type<K>::type *>> all_storers;
	return all_storers;
}

// storage for an object of type T that is uniquely
// identified by the ptr template argument. I use this
// to store one std::function for every function pointer
template<typename T, typename P, P ptr>
struct unique_storer
{
	static T func;

	// this struct is responsible for making func accessible at runtime
	// by someone who has the pointer ptr
	struct runtime_access
	{
		runtime_access()
		{
			getUniqueStorers<P>().emplace_back(ptr, &func);
		}
	};
	static runtime_access record_pointer;
};
template<typename T, typename P, P ptr>
T unique_storer<T, P, ptr>::func;
template<typename T, typename P, P ptr>
typename unique_storer<T, P, ptr>::runtime_access unique_storer<T, P, ptr>::record_pointer;

template<typename P, P ptr>
void clear_lambda()
{
	unique_storer<typename meta::func_type<P>::type, P, ptr>::func = nullptr;
}

// call_stored_function is used to generate a function pointer that I can use
// to call the stored std::function in a unique_storer
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
