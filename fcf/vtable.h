#pragma once
#include "meta.h"

namespace fcf
{

// this generates 256 virtual functions
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

struct VTableCounter
{
	typedef size_t (VTableCounter::*func_type)() const;
	enum { base_counter = __COUNTER__ };
	FCF_GET256
	size_t get_index(func_type func) const
	{
		return (this->*func)();
	}
};

template <typename F>
size_t getVTableIndex(F f)
{
	// here is how it works: the many functions generated above each correspond
	// to one index in the vtable. when I use reinterpret_cast on the given
	// function it will call one of the functions generated above. and that
	// function tells me it's index
	VTableCounter vt;
	return vt.get_index(reinterpret_cast<VTableCounter::func_type>(f));
}

void * getVirtualFunctionAddress(void * classInstance, size_t index);
const void * getVirtualFunctionAddress(const void * classInstance, size_t index);

}

