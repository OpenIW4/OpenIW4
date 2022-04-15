#include "R.hpp"

#include <utils/memory/memory.hpp>

//THUNK : 0x0050B0D0
int R_PopRemoteScreenUpdate()
{
	return memory::call<int()>(0x0050B0D0)();
}