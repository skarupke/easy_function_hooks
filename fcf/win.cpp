#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace fcf
{
void makeUnsafe(void * ptr, size_t num_bytes)
{
	DWORD old_protection;
	VirtualProtect(ptr, num_bytes, PAGE_EXECUTE_READWRITE, &old_protection);
}
void notifyCodeChange(const void * ptr, size_t num_bytes)
{
	FlushInstructionCache(GetCurrentProcess(), ptr, num_bytes);
}
}
