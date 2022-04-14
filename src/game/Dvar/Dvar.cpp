#include "Dvar.hpp"

#include <utils/memory/memory.hpp>

//THUNK : 0x004D8220
void Dvar_Init()
{
	memory::call<void()>(0x004D8220)();
}