#include "Cbuf.hpp"

#include <utils/memory/memory.hpp>

//THUNK : 0x00404B20
void Cbuf_AddText(int a1, const char* a2)
{
	memory::call<void(int, const char*)>(0x00404B20)(a1, a2);
}