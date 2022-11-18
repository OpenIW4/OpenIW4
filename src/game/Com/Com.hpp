#pragma once

#include "../stdafx.hpp"
#include "../defs.hpp"

#pragma region Com Variables
static ParseThreadInfo* g_parse = reinterpret_cast<ParseThreadInfo*>(0x6466628); //4 elements
static const char* String = *reinterpret_cast<const char**>(0x6FAC0D); //used a lot, not sure as to what it is

static const dvar_t* sv_running = *reinterpret_cast<const dvar_t**>(0x1AD7934);
static const dvar_t* sv_map = *reinterpret_cast<const dvar_t**>(0x2098DDC);
static std::int32_t* com_errorPrintsCount = reinterpret_cast<std::int32_t*>(0x1AD7910);
static std::int32_t com_fixedConsolePosition = *reinterpret_cast<std::int32_t*>(0x1AD8EC8);
static const char** puncuation = reinterpret_cast<const char**>(0x79D208);
#pragma endregion Com Variables

void Com_InitParse();
void Com_Frame();
void Com_Init(char* src);
void Com_Init_Cont();
void Com_Frame_Try_Block_Function();
void Com_EnterError();
int Com_ErrorCleanup();
int Com_StartHunkUsers();
void Com_AddStartupCommands();
//void Com_Error(errorParm_t code, const char* format, ...);
void Com_Printf(std::uint32_t channel, const char* Format, ...);
void Com_PrintError(int channel, const char* fmt, ...);
int Com_sprintf(char* buf, size_t bufCount, const char* fmt, ...);
void Com_Memcpy(void* dest, const void* src, int size);
void Com_Memset(void* dest, int val, int count);
ParseThreadInfo* Com_GetParseThreadInfo();
void Com_BeginParseSession(const char* filename);
void Com_InitParseInfo(parseInfo_t* pi);
void Com_EndParseSession();
char* Com_Parse(const char** data_p);
parseInfo_t* Com_ParseExt(const char** data_p, std::int32_t allowLineBreaks);
parseInfo_t* Com_ParseCSV(const char** data_p, std::int32_t allowLineBreaks);
const char* SkipWhitespace(const char* data, std::int32_t* newLines); //IDA is weird with this one
void Com_TouchMemory();
bool Com_IsRunningMenuLevel();


int I_strlen(const char* s);
char I_CleanChar(char c);
bool I_isdigit(int c);
bool I_islower(int c);
int I_strncmp(const char* s0, const char* s1, int n);
int I_strcmp(const char* s0, const char* s1);
std::int32_t I_strnicmp(const char* s0, const char* s1, std::int32_t n);
int I_stricmp(const char* s0, const char* s1);
void I_strncat(char* dest, int size, const char* src);
char* I_strlwr(char* s);
void I_strncpyz(char* dest, const char* src, int destsize);
char* I_CleanStr(char* string);
int I_DrawStrlen(const char* str);
const char* I_stristr(const char* s0, const char* substr);

char* va(const char* format, ...);
