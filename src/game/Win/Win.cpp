#include "Win.hpp"

#include <utils/memory/memory.hpp>

//THUNK : 0x005C8440
bool __cdecl Win_InitLocalization(language_t* language)
{
	return memory::call<bool(language_t*)>(0x005C8440)(language);
}

//DONE : 0x005C8500
void __cdecl Win_ShutdownLocalization()
{
	*(char*)(0x06428EB8) = 0;	//__ZL12localization_0
	*(char*)(0x06428EBC) = 0;	//__ZL12localization_1
}