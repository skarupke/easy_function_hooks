#pragma once

#include "fcf/lambda.h"
#include "fcf/function.h"
#include "fcf/vtable.h"
#include <cstdint>

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
	typedef ::fcf::meta::remove_ref_cv<decltype(x)>::type function_type;\
	::fcf::clear_lambda<function_type, x>();\
	::fcf::Assigner<function_type, x, typename ::fcf::meta::remove_ref_cv<decltype(rhs)>::type>::assign(std::forward<decltype(rhs)>(rhs));\
}\
else static_cast<void>(0)

#define FCF_VIRTUAL_ASSIGN(object, member_function, ...)\
if (true)\
{\
	auto && rhs = __VA_ARGS__;\
	typedef ::fcf::meta::remove_ref_cv<decltype(member_function)>::type function_type;\
	::fcf::clear_lambda<function_type, member_function>();\
	::fcf::VirtualAssigner<function_type, member_function, typename ::fcf::meta::remove_ref_cv<decltype(rhs)>::type>::assign(object, std::forward<decltype(rhs)>(rhs));\
}\
else static_cast<void>(0)


inline void place_jmp(void * from, const void * to)
{
	static const unsigned char JMP_INSTRUCTION = 0xE9;
	ptrdiff_t jump_dist = reinterpret_cast<size_t>(to) - reinterpret_cast<size_t>(from) - JMP_BYTE_COUNT;
	int32_t jump_dist_four_bytes = static_cast<int32_t>(jump_dist);
	// must be within two gigabytes of each other, because we only have
	// four bytes for the jump distance, even in 64 bit
	FCF_ASSERT(jump_dist_four_bytes == jump_dist);
	unsigned char * target_bytes = static_cast<unsigned char *>(from);
	makeUnsafe(target_bytes, JMP_BYTE_COUNT);
	target_bytes[0] = JMP_INSTRUCTION;
	for (int i = 0; i < sizeof(jump_dist_four_bytes); ++i) target_bytes[i + 1] = reinterpret_cast<unsigned char *>(&jump_dist_four_bytes)[i];
	notifyCodeChange(target_bytes, JMP_BYTE_COUNT);
}

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
	static void assign(T rhs)
	{
		if (lhs == rhs) return;
		place_jmp(lhs, rhs);
	}
};
template<typename T, T lhs>
struct MemberFunctionAssigner
{
	static void assign(T rhs)
	{
		if (lhs == rhs) return;
		place_jmp(getMemberFunctionAddress(lhs), getMemberFunctionAddress(rhs));
	}
};
template<typename T, T lhs>
struct VirtualFunctionAssigner
	: public VirtualBaseAssigner<T, lhs>
{
	static void assign(class_type & object, T rhs)
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
		rhs.restoreState<lhs>();
	}
};
template<typename T, T lhs>
struct VirtualFcfFunctionAssigner
	: public VirtualBaseAssigner<T, lhs>
{
	static void assign(class_type &, const Function<T> & rhs)
	{
		rhs.restoreState<lhs>();
	}
};

template<typename T, T lhs, typename F>
struct FunctorAssigner
{
	static void assign(F rhs)
	{
		typedef typename meta::func_type<T>::type func_type;
		// this next line is to force the compiler to instantiate the record_pointer
		// member from the unique storer so that it takes care of making the stored
		// function accessible at runtime
		static_cast<void>(unique_storer<func_type, T, lhs>::record_pointer);
		auto caller = &call_stored_function<func_type>::caller<func_type>::call<&unique_storer<func_type, T, lhs>::func>;
		FCF_ASSIGN(lhs, caller);
		unique_storer<func_type, T, lhs>::func = std::move(rhs);
	}
};

template<typename T, T lhs, typename F>
struct MemberFunctorAssigner
{
	static void assign(F rhs)
	{
		typedef typename meta::func_type<T>::type func_type;
		// this next line is to force the compiler to instantiate the record_pointer
		// member from the unique storer so that it takes care of making the stored
		// function accessible at runtime
		static_cast<void>(unique_storer<func_type, T, lhs>::record_pointer);
		auto caller = &member_call_stored_function<func_type>::caller<func_type>::call<&unique_storer<func_type, T, lhs>::func>;
		FCF_ASSIGN(lhs, caller);
		unique_storer<func_type, T, lhs>::func = std::move(rhs);
	}
};
template<typename T, T lhs, typename F>
struct VirtualFunctorAssigner
	: public VirtualBaseAssigner<T, lhs>
{
	static void assign(class_type & object, F rhs)
	{
		typedef typename meta::func_type<T>::type func_type;
		// this next line is to force the compiler to instantiate the record_pointer
		// member from the unique storer so that it takes care of making the stored
		// function accessible at runtime
		static_cast<void>(unique_storer<func_type, T, lhs>::record_pointer);
		auto caller = &member_call_stored_function<func_type>::caller<func_type>::call<&unique_storer<func_type, T, lhs>::func>;
		FCF_VIRTUAL_ASSIGN(object, lhs, caller);
		unique_storer<func_type, T, lhs>::func = std::move(rhs);
	}
};

template<typename T, T lhs, typename F>
struct CompatibleMemberFunctionAssigner
{
	static void assign(F rhs)
	{
		place_jmp(getMemberFunctionAddress(lhs), getMemberFunctionAddress(rhs));
	}
};

template<typename T, T lhs, typename F>
struct CompatibleVirtualFunctionAssigner
	: public VirtualBaseAssigner<T, lhs>
{
	static void assign(class_type & object, F rhs)
	{
		place_jmp(getFunctionAddress(object), getMemberFunctionAddress(rhs));
	}
};

template<typename T, T lhs, typename AssignTo>
struct Assigner
	: std::conditional
	<
		std::is_same<Function<T>, AssignTo>::value,
		FcfFunctionAssigner<T, lhs>,
		typename std::conditional
		<
			meta::is_method<T>::value,
			typename std::conditional
			<
				std::is_same<T, AssignTo>::value,
				MemberFunctionAssigner<T, lhs>,
				typename std::conditional
				<
					meta::is_method_compatible<T, AssignTo>::value,
					CompatibleMemberFunctionAssigner<T, lhs, AssignTo>,
					MemberFunctorAssigner<T, lhs, AssignTo>
				>::type
			>::type,
			typename std::conditional
			<
				std::is_same<T, AssignTo>::value,
				FunctionPointerAssigner<T, lhs>,
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
			VirtualFcfFunctionAssigner<T, lhs>,
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
