#pragma once

#include "../stdafx.hpp"

void Com_InitParse();
void Com_Frame();
void Com_Init(char* src);
void Com_Frame_Try_Block_Function();
bool Com_EnterError();
int Com_LeaveError();
int Com_ErrorCleanup();
int Com_StartHunkUsers();