#pragma once

#include "../stdafx.hpp"
#include "../defs.hpp"

bool Win_InitLocalization(language_t* language);
void Win_ShutdownLocalization();
const char* Win_GetLocalizationFilename();
const char* Win_LocalizeRef(const char* ref);
