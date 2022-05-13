#include "Cmd.hpp"

#include <memory/memory.hpp>

//THUNK : 0x00470090
int Cmd_AddCommandInternal(const char* s1, void(* callback)(), void* allocation, bool key)
{
	return memory::call<int(const char*, void(*)(), void*, bool)>(0x00470090)(s1, callback, allocation, key);
}