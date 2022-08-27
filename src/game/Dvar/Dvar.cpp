#include "Dvar.hpp"

#include <utils/memory/memory.hpp>

//THUNK : 0x004D8220
void Dvar_Init()
{
	memory::call<void()>(0x004D8220)();
}

//THUNK : 0x00648440
int Dvar_RegisterFloat(char* command_name, float default_value, float minimum_value, float maximum_value, int flags, char* command_description)
{
	return memory::call<int(char*, float, float, float, int, char*)>(0x00648440)
		(command_name, default_value, minimum_value, maximum_value, flags, command_description);
}

//THUNK : 0x004CE1A0
int Dvar_RegisterBool(const char* command_name, bool default_value, unsigned int maximum_value, const char* command_description)
{
	return memory::call<int(const char*, bool, unsigned int, const char*)>(0x00648440)
		(command_name, default_value, maximum_value, command_description);
}
