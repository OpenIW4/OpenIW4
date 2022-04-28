#pragma once

#include "../stdafx.hpp"

int Cmd_AddCommandInternal(const char* s1, void(*callback)(), void* allocation, bool key);