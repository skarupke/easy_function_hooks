#include "../fcf.h"
#include <cstdlib>
#include <cstdint>
#include "os.h"

namespace fcf
{

namespace { static const unsigned char JMP = 0xE9; }

void place_jmp(void * from, const void * to)
{
#ifdef _DEBUG
	// must be within two gigabytes of each other
	ptrdiff_t distance = static_cast<const char *>(from) - static_cast<const char *>(to);
	FCF_ASSERT(static_cast<int32_t>(distance) == distance);
#endif
	unsigned char jmp_bytes[JMP_BYTE_COUNT];
	jmp_bytes[0] = JMP;
	uint32_t jump_dist = reinterpret_cast<uint32_t>(to) - reinterpret_cast<uint32_t>(from) - sizeof(jmp_bytes);
	memcpy(jmp_bytes + 1, &jump_dist, sizeof(jump_dist));
	makeUnsafe(from, sizeof(jmp_bytes));
	memcpy(from, jmp_bytes, sizeof(jmp_bytes));
	notifyCodeChange(from, sizeof(jmp_bytes));
}

void * getVirtualFunctionAddress(void * classInstance, size_t index)
{
	void ** vTable = *reinterpret_cast<void ***>(classInstance);
	return vTable[index];
}

const void * getVirtualFunctionAddress(const void * classInstance, size_t index)
{
	const void ** vTable = *reinterpret_cast<const void ** const *>(classInstance);
	return vTable[index];
}

}
