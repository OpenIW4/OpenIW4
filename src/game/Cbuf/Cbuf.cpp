#include "Cbuf.hpp"

#include <utils/memory/memory.hpp>

//THUNK : 0x00404B20
void Cbuf_AddText(int a1, const char* a2)
{
	memory::call<void(int, const char*)>(0x00404B20)(a1, a2);
}

//THUNK : 0x4AFB80
void Con_GetTextCopy(char* text, int maxSize)
{
    memory::call<void(char*, int)>(0x4AFB80)(text, maxSize);
}

//THUNK : 0x64DD30
int Conbuf_CleanText(const char* source, char* target, int sizeOfTarget)
{
    return memory::call<int(const char*, char*, int)>(0x64DD30)(source, target, sizeOfTarget);
}