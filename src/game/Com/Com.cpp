#include "Com.hpp"

#include <utils/memory/memory.hpp>

//THUNK : 0x005C0AB0
void Com_InitParse()
{
	memory::call<void()>(0x005C0AB0)();
}

//THUNK : 0x0056B4B0
void Com_Frame()
{
	memory::call<void()>(0x0056B4B0)();
}