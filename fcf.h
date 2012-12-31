#pragma once

#include "fcf/lambda.h"
#include "fcf/function.h"
#include "fcf/vtable.h"

namespace fcf
{
template<typename T>
Function<T> function(T func)
{
	return Function<T>(func);
}
template<typename T>
VirtualFunction<T> virtual_function(typename meta::class_type<T>::type & object, T func)
{
	return VirtualFunction<T>(object, func);
}

#define FCF_ASSIGN(x, ...)\
if (true)\
{\
	auto && rhs = __VA_ARGS__;\
	::fcf::Assigner<typename ::fcf::meta::remove_ref_cv<decltype(x)>::type, x, typename ::fcf::meta::remove_ref_cv<decltype(rhs)>::type>::assign(rhs);\
}\
else static_cast<void>(0)

#define FCF_VIRTUAL_ASSIGN(object, member_function, ...)\
if (true)\
{\
	auto && rhs = __VA_ARGS__;\
	::fcf::VirtualAssigner<typename ::fcf::meta::remove_ref_cv<decltype(member_function)>::type, member_function, typename ::fcf::meta::remove_ref_cv<decltype(rhs)>::type>::assign(object, rhs);\
}\
else static_cast<void>(0)


void place_jmp(void * from, const void * to);

template<typename T, T lhs>
struct VirtualBaseAssigner
{
	typedef typename meta::class_type<T>::type class_type;
	static void * getFunctionAddress(class_type & object)
	{
		return getVirtualFunctionAddress(&object, getVTableIndex<T>(lhs));
	}
};

template<typename T, T lhs>
struct FunctionPointerAssigner
{
	static void assign(const T rhs)
	{
		if (lhs == rhs) return;
		place_jmp(lhs, rhs);
	}
};
template<typename T, T lhs>
struct MemberFunctionAssigner
{
	static void assign(const T rhs)
	{
		if (lhs == rhs) return;
		place_jmp(getMemberFunctionAddress(lhs), getMemberFunctionAddress(rhs));
	}
};
template<typename T, T lhs>
struct VirtualFunctionAssigner
	: public VirtualBaseAssigner<T, lhs>
{
	static void assign(class_type & object, const T rhs)
	{
		if (lhs == rhs) return;
		place_jmp(getFunctionAddress(object), getMemberFunctionAddress(rhs));
	}
};

template<typename T, T lhs>
struct FcfFunctionAssigner
{
	static void assign(const Function<T> & rhs)
	{
		// this assert is here because you can only assign a function to a fcf::Function that
		// has information about itself. so only this is valid:
		// fcf::Function old = foo;
		// fcf::assign(foo) = bar;
		// fcf::assign(foo) = old;
		FCF_ASSERT(lhs == rhs.get());
		memcpy(lhs, rhs.get_start_bytes(), JMP_BYTE_COUNT);
	}
};
template<typename T, T lhs>
struct MemberFcfFunctionAssigner
{
	static void assign(const Function<T> & rhs)
	{
		// this assert is here because you can only assign a function to a fcf::Function that
		// has information about itself. so only this is valid:
		// fcf::Function old = foo; // store original value
		// fcf::assign(foo) = bar;
		// fcf::assign(foo) = old; // assign back to original value
		FCF_ASSERT(lhs == rhs.get());
		memcpy(getMemberFunctionAddress(lhs), rhs.get_start_bytes(), JMP_BYTE_COUNT);
	}
};
template<typename T, T lhs>
struct VirtualFofFunctionAssigner
	: public VirtualBaseAssigner<T, lhs>
{
	static void assign(class_type & object, const Function<T> & rhs)
	{
		// this assert is here because you can only assign a function to a fcf::Function that
		// has information about itself. so only this is valid:
		// fcf::Function old = foo; // store original value
		// fcf::assign(foo) = bar;
		// fcf::assign(foo) = old; // assign back to original value
		FCF_ASSERT(lhs == rhs.get());
		memcpy(getFunctionAddress(object), rhs.get_start_bytes(), JMP_BYTE_COUNT);
	}
};

template<typename T, T lhs, typename F>
struct FunctorAssigner
{
	static void assign(const F & rhs)
	{
		typedef typename meta::func_type<T>::type func_type;
		unique_storer<func_type, T, lhs>::func = rhs;
		FCF_ASSIGN(lhs, &call_stored_function<meta::calling_convention<T>::value, func_type, &unique_storer<func_type, T, lhs>::func>::call);
	}
};
template<typename T, T lhs, typename F>
struct MemberFunctorAssigner
{
	static void assign(const F & rhs)
	{
		typedef typename meta::func_type<T>::type func_type;
		unique_storer<func_type, T, lhs>::func = rhs;
		FCF_ASSIGN(lhs, &member_call_stored_function<meta::calling_convention<T>::value, func_type, &unique_storer<func_type, T, lhs>::func>::call);
	}
};
template<typename T, T lhs, typename F>
struct VirtualFunctorAssigner
	: public VirtualBaseAssigner<T, lhs>
{
	static void assign(class_type & object, const F & rhs)
	{
		typedef typename meta::func_type<T>::type func_type;
		unique_storer<func_type, T, lhs>::func = rhs;
		FCF_VIRTUAL_ASSIGN(object, lhs, &member_call_stored_function<meta::calling_convention<T>::value, func_type, &unique_storer<func_type, T, lhs>::func>::call);
	}
};

template<typename T, T lhs, typename F>
struct CompatibleMemberFunctionAssigner
{
	static void assign(const F rhs)
	{
		place_jmp(getMemberFunctionAddress(lhs), getMemberFunctionAddress(rhs));
	}
};

template<typename T, T lhs, typename F>
struct CompatibleVirtualFunctionAssigner
	: public VirtualBaseAssigner<T, lhs>
{
	static void assign(class_type & object, const F rhs)
	{
		place_jmp(getFunctionAddress(object), getMemberFunctionAddress(rhs));
	}
};

template<typename T, T lhs, typename AssignTo>
struct Assigner
	: std::conditional
	<
		meta::is_method<T>::value,
		typename std::conditional
		<
			std::is_same<T, AssignTo>::value,
			MemberFunctionAssigner<T, lhs>,
			typename std::conditional
			<
				std::is_same<Function<T>, AssignTo>::value,
				MemberFcfFunctionAssigner<T, lhs>,
				typename std::conditional
				<
					meta::is_method_compatible<T, AssignTo>::value,
					CompatibleMemberFunctionAssigner<T, lhs, AssignTo>,
					MemberFunctorAssigner<T, lhs, AssignTo>
				>::type
			>::type
		>::type,
		typename std::conditional
		<
			std::is_same<T, AssignTo>::value,
			FunctionPointerAssigner<T, lhs>,
			typename std::conditional
			<
				std::is_same<Function<T>, AssignTo>::value,
				FcfFunctionAssigner<T, lhs>,
				FunctorAssigner<T, lhs, AssignTo>
			>::type
		>::type
	>::type
{
};

template<typename T, T lhs, typename AssignTo>
struct VirtualAssigner
	: std::conditional
	<
		std::is_same<T, AssignTo>::value,
		VirtualFunctionAssigner<T, lhs>,
		typename std::conditional
		<
			std::is_same<Function<T>, AssignTo>::value,
			VirtualFofFunctionAssigner<T, lhs>,
			typename std::conditional
			<
				meta::is_method_compatible<T, AssignTo>::value,
				CompatibleVirtualFunctionAssigner<T, lhs, AssignTo>,
				VirtualFunctorAssigner<T, lhs, AssignTo>
			>::type
		>::type
	>::type
{
};

}
