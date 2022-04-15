#pragma once

#include "../stdafx.hpp"

void Dvar_Init();
int Dvar_RegisterFloat(char* command_name, float default_value, float minimum_value, float maximum_value, int a5, char* command_description);