#pragma once

#include "../stdafx.hpp"

typedef struct cmd_function_s
{
	cmd_function_s* next;
	const char* name;
	const char* autoCompleteDir;
	const char* autoCompleteExt;
	void(__cdecl* function)();
	int flags;
} cmd_function_t;

int Cmd_AddCommandInternal(const char* s1, void(*callback)(), void* allocation, bool key);