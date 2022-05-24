#pragma once

#include "../stdafx.hpp"
#include "game/defs.hpp"

void Com_InitParse();
void Com_Frame();
void Com_Init(char* src);
void Com_Init_Cont();
void Com_Frame_Try_Block_Function();
bool Com_EnterError();
int Com_ErrorCleanup();
int Com_StartHunkUsers();
void Com_AddStartupCommands();
//void Com_Error(errorParm_t code, const char* format, ...);
void Com_Printf(std::uint32_t channel, const char* Format, ...);
void Com_PrintError(int channel, const char* fmt, ...);
int Com_sprintf(char* buf, size_t bufCount, const char* fmt, ...);
void Com_Memcpy(void* dest, const void* src, int size);
void Com_Memset(void* dest, int val, int count);

int I_strlen(const char* s);
char I_CleanChar(char c);
bool I_isdigit(int c);
bool I_islower(int c);
int I_strncmp(const char* s0, const char* s1, int n);
int I_strcmp(const char* s0, const char* s1);
int I_strnicmp(const char* s0, const char* s1, int n);
int I_stricmp(const char* s0, const char* s1);
void I_strncat(char* dest, int size, const char* src);
char* I_strlwr(char* s);
void I_strncpyz(char* dest, const char* src, int destsize);
char* I_CleanStr(char* string);
int I_DrawStrlen(const char* str);
const char* I_stristr(const char* s0, const char* substr);

char* va(const char* format, ...);
