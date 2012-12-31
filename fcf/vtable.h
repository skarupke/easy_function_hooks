#pragma once
#include "meta.h"

namespace fcf
{

#define FCF_GET(callingConvention, x) virtual size_t callingConvention Get ## x() const { return x - 1 - base_counter; }
#define FCF_GET_CAT(cc, x) FCF_GET(cc, x)
#define FCF_GET_COUNTER(cc) FCF_GET_CAT(cc, __COUNTER__)
#define FCF_GET2(cc) FCF_GET_COUNTER(cc) FCF_GET_COUNTER(cc)
#define FCF_GET4(cc) FCF_GET2(cc) FCF_GET2(cc)
#define FCF_GET8(cc) FCF_GET4(cc) FCF_GET4(cc)
#define FCF_GET16(cc) FCF_GET8(cc) FCF_GET8(cc)
#define FCF_GET32(cc) FCF_GET16(cc) FCF_GET16(cc)
#define FCF_GET64(cc) FCF_GET32(cc) FCF_GET32(cc)
#define FCF_GET128(cc) FCF_GET64(cc) FCF_GET64(cc)
#define FCF_GET256(cc) FCF_GET128(cc) FCF_GET128(cc)

template<typename T, typename Enable = void>
struct VTableCounter;

#ifdef _M_X64
template<typename T>
struct VTableCounter<T, typename std::enable_if<FCF_CALL_CONVENTION == meta::X64Call>::type>
{
	typedef size_t (__cdecl VTableCounter::*func_type)() const;
	enum { base_counter = __COUNTER__ };
	FCF_GET256(__cdecl)
	size_t __thiscall call(func_type func)
	{
		return (this->*func)();
	}
};
#else
template<typename T>
struct VTableCounter<T, typename std::enable_if<FCF_CALL_CONVENTION == meta::CDecl>::type>
{
	typedef size_t (__cdecl VTableCounter::*func_type)() const;
	enum { base_counter = __COUNTER__ };
	FCF_GET256(__cdecl)
	size_t __thiscall call(func_type func)
	{
		return (this->*func)();
	}
};
template<typename T>
struct VTableCounter<T, typename std::enable_if<FCF_CALL_CONVENTION == meta::StdCall>::type>
{
	typedef size_t (__stdcall VTableCounter::*func_type)() const;
	enum { base_counter = __COUNTER__ };
	FCF_GET256(__stdcall)
	size_t __thiscall call(func_type func)
	{
		return (this->*func)();
	}
};
template<typename T>
struct VTableCounter<T, typename std::enable_if<FCF_CALL_CONVENTION == meta::FastCall>::type>
{
	typedef size_t (__fastcall VTableCounter::*func_type)() const;
	enum { base_counter = __COUNTER__ };
	FCF_GET256(__fastcall)
	size_t __thiscall call(func_type func)
	{
		return (this->*func)();
	}
};
template<typename T>
struct VTableCounter<T, typename std::enable_if<FCF_CALL_CONVENTION == meta::ThisCall>::type>
{
	typedef size_t (__thiscall VTableCounter::*func_type)() const;
	enum { base_counter = __COUNTER__ };
	FCF_GET256(__thiscall)
	size_t __thiscall call(func_type func)
	{
		return (this->*func)();
	}
};
#endif

template <typename F>
size_t getVTableIndex(F f)
{
	VTableCounter<F> vt;
	return vt.call(reinterpret_cast<typename VTableCounter<F>::func_type>(f));
}

void * getVirtualFunctionAddress(void * classInstance, size_t index);
const void * getVirtualFunctionAddress(const void * classInstance, size_t index);

}

