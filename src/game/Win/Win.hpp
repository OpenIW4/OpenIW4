#pragma once

#include "../stdafx.hpp"
#include "../defs.hpp"

static localization_t* localization = reinterpret_cast<localization_t*>(0x649E740);

bool Win_InitLocalization(language_t* language);
void Win_ShutdownLocalization();
const char* Win_GetLocalizationFilename();
const char* Win_LocalizeRef(const char* ref);
