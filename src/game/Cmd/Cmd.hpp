#pragma once

#include "../stdafx.hpp"
#include "../defs.hpp"

cmd_function_s* Cmd_FindCommand(const char* cmdName);
void Cmd_AddCommandInternal(const char* cmdName, void(__cdecl* function)(), cmd_function_s* allocedCmd, std::int32_t flags);