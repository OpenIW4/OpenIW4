#include "DB.hpp"

#include <utils/memory/memory.hpp>

//THUNK : 0x00440EC0
int DB_Update()
{
	return memory::call<int()>(0x00440EC0)();
}