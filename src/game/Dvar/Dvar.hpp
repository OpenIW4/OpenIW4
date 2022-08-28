#pragma once

#include "../stdafx.hpp"
#include "../defs.hpp"

void Dvar_Init();
const dvar_t* Dvar_RegisterFloat(char* command_name, float default_value, float minimum_value, float maximum_value, int flags, char* command_description);
const dvar_t* Dvar_RegisterBool(const char* a1, bool a2, unsigned int a3, const char* a4);
const dvar_t* Dvar_RegisterInt(const char* dvarName, std::int32_t a2, std::int32_t a3, std::int32_t a4, std::int32_t a5, const char* description);
