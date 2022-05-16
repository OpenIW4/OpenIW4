#pragma once

#include "../stdafx.hpp"

void Com_InitParse();
void Com_Frame();
void Com_Init(char* src);
void Com_Init_Cont();
void Com_Frame_Try_Block_Function();
bool Com_EnterError();
int Com_ErrorCleanup();
int Com_StartHunkUsers();
void Com_AddStartupCommands();
//int Com_Error(errorParm_t code, char* format, ...);
void Com_Printf(std::uint32_t channel, const char* Format, ...);
int Com_sprintf(char* buf, size_t bufCount, const char* fmt, ...);
void* Com_Memcpy(void* dest, const void* src, int size);
