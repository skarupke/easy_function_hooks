#pragma once
#include "meta.h"

namespace fcf
{

#define FCF_GET(x) virtual size_t Get ## x() const { return x - 1 - base_counter; }
#define FCF_GET_CAT(x) FCF_GET(x)
#define FCF_GET2 FCF_GET_CAT(__COUNTER__) FCF_GET_CAT(__COUNTER__)
#define FCF_GET4 FCF_GET2 FCF_GET2
#define FCF_GET8 FCF_GET4 FCF_GET4
#define FCF_GET16 FCF_GET8 FCF_GET8
#define FCF_GET32 FCF_GET16 FCF_GET16
#define FCF_GET64 FCF_GET32 FCF_GET32
#define FCF_GET128 FCF_GET64 FCF_GET64
#define FCF_GET256 FCF_GET128 FCF_GET128

template<typename T>
struct VTableCounter
{
	typedef size_t (VTableCounter::*func_type)() const;
	enum { base_counter = __COUNTER__ };
	FCF_GET256
	size_t call(func_type func)
	{
		return (this->*func)();
	}
};

template <typename F>
size_t getVTableIndex(F f)
{
	VTableCounter<F> vt;
	return vt.call(reinterpret_cast<typename VTableCounter<F>::func_type>(f));
}

void * getVirtualFunctionAddress(void * classInstance, size_t index);
const void * getVirtualFunctionAddress(const void * classInstance, size_t index);

}

