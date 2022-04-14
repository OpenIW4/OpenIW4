#include "Com.hpp"

#include <utils/memory/memory.hpp>

//THUNK : 0x004FF220
void Com_InitParse()
{
	memory::call<void()>(0x004FF220)();
}

//THUNK : 0x004B7230
int Com_Frame()
{
	return memory::call<int()>(0x004B7230)();
}

//THUNK : 0x004D9640
void Com_Init(char* src)
{
	memory::call<void(char*)>(0x004D9640)(src);
}