#pragma once

#include "../stdafx.hpp"

static scrMemTreePub_t* scrMemTreePub = reinterpret_cast<scrMemTreePub_t*>(0x1DC2200);

uint32_t SL_GetString_(const char* str, uint32_t user, int type);
char* SL_ConvertToString(uint32_t stringValue);
const char* CopyString(const char* in);
