#pragma once

#include "../stdafx.hpp"

LRESULT Sys_ShowConsole();
ATOM Sys_CreateConsole(HINSTANCE hInstance);
void Sys_InitializeCriticalSections();
void Sys_InitMainThread();
void Sys_Sleep(DWORD dwMilliseconds);
void Sys_CheckQuitRequest();
void Sys_RecordAccessibilityShortcutSettings();
void Sys_AllowAccessibilityShortcutKeys(bool a1)