#include "Win.hpp"

#include <utils/memory/memory.hpp>

//THUNK : 0x00406D10
bool Win_InitLocalization(language_t* language)
{
	return memory::call<bool(language_t*)>(0x00406D10)(language);
}

//DONE : 0x0040C6D0
void Win_ShutdownLocalization()
{
	*(char*)(0x06428EB8) = 0;	//__ZL12localization_0
	*(char*)(0x06428EBC) = 0;	//__ZL12localization_1
}

//DONE : 0x004F7950
const char* Win_GetLocalizationFilename()
{
	return "localization.txt";
}
