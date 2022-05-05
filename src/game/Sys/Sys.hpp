#pragma once

#include "../stdafx.hpp"

void Sys_ShowConsole();
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
void Sys_EnterCriticalSection(int a1);
void Sys_LeaveCriticalSection(int a1);
void Sys_EnumerateHw();
bool Sys_IsDatabaseReady();
bool Sys_IsDatabaseReady2();
bool Sys_ReleaseThreadOwnership();
bool Sys_DatabaseCompleted();
long __stdcall ConsoleWndProc(HWND hWnd, std::uint32_t msg, std::uint32_t wParam, long lParam);
long __stdcall InputLineWndProc(HWND hWnd, std::uint32_t msg, std::uint32_t wParam, long lParam);
void Sys_Error(const char* error, ...);
unsigned long Sys_SuspendOtherThreads();
void Sys_SetErrorText(const char* text);