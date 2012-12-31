#include "../fcf.h"
#include "os.h"

namespace fcf
{

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
