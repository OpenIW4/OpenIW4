#include "Dvar.hpp"

#include <utils/memory/memory.hpp>

//THUNK : 0x005BE9C0
void Dvar_Init()
{
	memory::call<void()>(0x005BE9C0)();
}