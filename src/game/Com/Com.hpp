#pragma once

#include "../stdafx.hpp"

enum errorParm_t
{
	ERR_FATAL = 0x0,
	ERR_DROP = 0x1,
	ERR_SERVERDISCONNECT = 0x2,
	ERR_DISCONNECT = 0x3,
	ERR_SCRIPT = 0x4,
	ERR_SCRIPT_DROP = 0x5,
	ERR_LOCALIZATION = 0x6,
	ERR_MAPLOADERRORSUMMARY = 0x7,
};

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
void Com_Printf(std::uint32_t channel, char* Format, ...);