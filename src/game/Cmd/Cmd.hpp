#pragma once

#include "../stdafx.hpp"
#include "../defs.hpp"

static cmd_function_s** cmd_functions = reinterpret_cast<cmd_function_s**>(0x1AAC658);

cmd_function_s* Cmd_FindCommand(const char* cmdName);
void Cmd_AddCommandInternal(const char* cmdName, void(__cdecl* function)(), cmd_function_s* allocedCmd, bool isKey);
void Cmd_RemoveCommand(const char* cmdName);
void Cmd_SetAutoComplete(const char* cmdName, const char* dir, const char* ext);
