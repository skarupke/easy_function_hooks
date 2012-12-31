#pragma once

namespace fcf
{
void makeUnsafe(void * ptr, size_t num_bytes);
void notifyCodeChange(const void * ptr, size_t num_bytes);
static const size_t JMP_BYTE_COUNT = 5;

template<typename T>
void * getMemberFunctionAddress(T mem_ptr)
{
	union as_void
	{
		as_void(T mem_ptr) : mem_ptr(mem_ptr) {}
		T mem_ptr;
		void * ptr;
	};
	return as_void(mem_ptr).ptr;
}
#ifdef _DEBUG
#define FCF_ASSERT(cond) if (!(cond)) __debugbreak(); else static_cast<void>(0)
#else
#define FCF_ASSERT(cond)
#endif
}
