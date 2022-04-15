#include "Dvar.hpp"

#include <utils/memory/memory.hpp>

//THUNK : 0x004D8220
void Dvar_Init()
{
	memory::call<void()>(0x004D8220)();
}

//THUNK : 0x00648440
int Dvar_RegisterFloat(char* command_name, float default_value, float minimum_value, float maximum_value, int a5, char* command_description)
{
	return memory::call<int(char*, float, float, float, int, char*)>(0x00648440)
		(command_name, default_value, minimum_value, maximum_value, a5, command_description);
}