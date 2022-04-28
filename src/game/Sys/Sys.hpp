#pragma once

#include "../stdafx.hpp"

LRESULT Sys_ShowConsole();
void Sys_CreateConsole(HINSTANCE hInstance);
void Sys_InitializeCriticalSections();
void Sys_InitMainThread();
void Sys_Sleep(DWORD dwMilliseconds);
void Sys_CheckQuitRequest();
void Sys_RecordAccessibilityShortcutSettings();
void Sys_AllowAccessibilityShortcutKeys(bool a1);
void Sys_CreateSplashWindow();
void Sys_ShowSplashWindow();
void Sys_RegisterClass();
int Sys_Milliseconds();
int* Sys_GetValue(int a1);
void Sys_getcwd();
bool Sys_IsMainThread();
void Sys_EnterCriticalSection(int);
void Sys_LeaveCriticalSection(int);
void Sys_EnumerateHw();
bool Sys_IsDatabaseReady();
bool Sys_IsDatabaseReady2();
void Sys_Error(char* Format, ...);
bool Sys_ReleaseThreadOwnership();
BOOL Sys_DatabaseCompleted();