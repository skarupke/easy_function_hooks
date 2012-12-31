#pragma once

#define FCF_LOOKUP_SYMBOL(symbol)\
	[]() -> void *\
	{\
		void * toreturn = nullptr;\
		{_asm mov eax, symbol _asm mov toreturn, eax }\
		return toreturn;\
	}()


