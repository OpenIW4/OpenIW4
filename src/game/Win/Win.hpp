#pragma once

#include "../stdafx.hpp"

struct language_t
{
	char* loc_string;
};

bool Win_InitLocalization(language_t* language);
void Win_ShutdownLocalization();
const char* Win_GetLocalizationFilename();