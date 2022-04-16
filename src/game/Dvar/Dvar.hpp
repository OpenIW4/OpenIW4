#pragma once

#include "../stdafx.hpp"

void Dvar_Init();
int Dvar_RegisterFloat(char* command_name, float default_value, float minimum_value, float maximum_value, int a5, char* command_description);
int Dvar_RegisterBool(const char* a1, bool a2, unsigned int a3, const char* a4);