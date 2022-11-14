#pragma once

#include "../stdafx.hpp"
#include "../defs.hpp"

static FastCriticalSection dvarCritSect = *reinterpret_cast<FastCriticalSection*>(0x63EF334);
static dvar_t** dvarHashTable = *reinterpret_cast<dvar_t***>(0x63F3350); //1024 elements

void Dvar_Init();
const dvar_t* Dvar_RegisterFloat(char* command_name, float default_value, float minimum_value, float maximum_value, int flags, char* command_description);
const dvar_t* Dvar_RegisterBool(const char* a1, bool a2, unsigned int a3, const char* a4);
const dvar_t* Dvar_RegisterInt(const char* dvarName, std::int32_t a2, std::int32_t a3, std::int32_t a4, std::int32_t a5, const char* description);

dvar_t* Dvar_FindMalleableVar(const char* dvarName);

const char* Dvar_AllocNameString(const char* name);
