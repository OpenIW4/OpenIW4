#pragma once

#include "../stdafx.hpp"

struct language_t
{
	char* loc_string;
};

bool __cdecl Win_InitLocalization(language_t* language);
void __cdecl Win_ShutdownLocalization();