#include "Sys.hpp"
#include "../Com/Com.hpp"

#include <utils/memory/memory.hpp>

//DONE : 0x004305E0
LRESULT Sys_ShowConsole()
{
    HMODULE v0;
    if (!*(HWND*)(0x064A3288))
    {
        v0 = GetModuleHandleA(0);
        Sys_CreateConsole(v0);
    }
    ShowWindow(*(HWND*)(0x064A3288), 1);
    return SendMessageA(*(HWND*)(0x0064A328C), 0xB6u, 0, 0xFFFF);
}

//THUNK : 0x004288A0
ATOM Sys_CreateConsole(HINSTANCE hInstance)
{
    return memory::call<ATOM(HINSTANCE)>(0x004288A0)(hInstance);
}

//THUNK : 0x0042F0A0
void Sys_InitializeCriticalSections()
{
    memory::call<void()>(0x0042F0A0)();
}

//THUNK : 0x004301B0
void Sys_InitMainThread()
{
    memory::call<void()>(0x004301B0)();
}

//DONE : 0x004169C0
void Sys_Sleep(DWORD dwMilliseconds)
{
    Sleep(dwMilliseconds);
}

//THUNK : 0x0043EBB0
void Sys_CheckQuitRequest()
{
    memory::call<void()>(0x0043EBB0)();
}

//THUNK : 0x004C8E30
void Sys_RecordAccessibilityShortcutSettings()
{
    memory::call<void()>(0x004C8E30)();
}

//THUNK : 0x0040BC60
void Sys_AllowAccessibilityShortcutKeys(bool a1)
{
    memory::call<void(bool)>(0x0040BC60)(a1);
}

//DONE : 0x00475F00
void Sys_CreateSplashWindow()
{
    int SystemMetrics; // edi
    int v1; // ebx
    HANDLE ImageA; // ebp
    HWND Window; // eax
    HWND v4; // eax
    HWND v5; // esi
    int v6; // ecx
    int v7; // esi
    struct tagRECT Rect; // [esp+0h] [ebp-38h] BYREF
    WNDCLASSA WndClass; // [esp+10h] [ebp-28h] BYREF

    WndClass.style = 0;
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.lpszMenuName = 0;
    WndClass.lpfnWndProc = DefWindowProcA;
    WndClass.hInstance = *(HINSTANCE*)(0x064A3AD4);
    WndClass.hIcon = LoadIconA(*(HINSTANCE*)(0x064A3AD4), (LPCSTR)1);
    WndClass.hCursor = LoadCursorA(0, (LPCSTR)0x7F00);
    WndClass.hbrBackground = (HBRUSH)6;
    WndClass.lpszClassName = "CoD Splash Screen";
    if (RegisterClassA(&WndClass))
    {
        SystemMetrics = GetSystemMetrics(16);
        v1 = GetSystemMetrics(17);
        ImageA = LoadImageA(0, "openiw4-splash.bmp", 0, 0, 0, 0x10u);
        if (ImageA)
        {
            memory::call<char()>(0x0045F8B0)();
            Window = CreateWindowExA(
                0x40000u,
                "CoD Splash Screen",
                "Modern Warfare 2 Multiplayer",
                0x80880000,
                (SystemMetrics - 320) / 2,
                (v1 - 100) / 2,
                320,
                100,
                0,
                0,
                *(HINSTANCE*)(0x064A3AD4),
                0);
            *(HWND*)(0x064A3050) /*g_splash_wv*/ = Window;
            if (Window)
            {
                v4 = CreateWindowExA(0, "Static", 0, 0x5000000Eu, 0, 0, 320, 100, Window, 0, *(HINSTANCE*)(0x064A3AD4), 0);
                v5 = v4;
                if (v4)
                {
                    SendMessageA(v4, 0x172u, 0, (LPARAM)ImageA);
                    GetWindowRect(v5, &Rect);
                    v6 = Rect.right - Rect.left + 2;
                    Rect.left = (SystemMetrics - v6) / 2;
                    v7 = Rect.bottom - Rect.top + 2;
                    Rect.top = (v1 - v7) / 2;
                    Rect.right = Rect.left + v6;
                    Rect.bottom = Rect.top + v7;
                    AdjustWindowRect(&Rect, 0x5000000Eu, 0);
                    SetWindowPos(*(HWND*)(0x064A3050) /*g_splash_wv*/, 0, Rect.left, Rect.top, Rect.right - Rect.left, Rect.bottom - Rect.top, 4u);
                }
            }
        }
    }
}

//DONE : 0x004A7B10
void Sys_ShowSplashWindow()
{
    if (*(HWND*)(0x064A3050)/*g_splash_wv*/)
    {
        ShowWindow(*(HWND*)(0x064A3050)/*g_splash_wv*/, 5);
        UpdateWindow(*(HWND*)(0x064A3050)/*g_splash_wv*/);
    }
}

//THUNK : 0x0064D270
void Sys_RegisterClass()
{
    memory::call<void()>(0x0064D270)();
}

//DONE : 0x0042A660
int Sys_Milliseconds()
{
    if (!*(int*)(0x064A304C)/*Sys_Milliseconds(void)::initialized*/)
    {
        *(int*)(0x064A3034) /*sys_timeBase*/ = timeGetTime();
        *(int*)(0x064A304C)/*Sys_Milliseconds(void)::initialized*/ = 1;
    }
    return timeGetTime() - *(int*)(0x064A3034) /*sys_timeBase*/;
}

//THUNK : 0x004EC730
int* Sys_GetValue(int a1)
{
    return memory::call<int*(int)>(0x004EC730)(a1);
}

//THUNK : 0x0064D300
void Sys_getcwd()
{
    memory::call<void()>(0x0064D300)();
}

//THUNK : 0x004C37D0
bool Sys_IsMainThread()
{
    return memory::call<bool()>(0x004C37D0)();
}

//DONE : 0x004FC200
void Sys_EnterCriticalSection(int a1)
{
    EnterCriticalSection((LPCRITICAL_SECTION)(24 * a1 + 0x6499BC8));
}

//DONE : 0x004FC200
void Sys_LeaveCriticalSection(int a1)
{
    LeaveCriticalSection((LPCRITICAL_SECTION)(24 * a1 + 105487304));
}

//THUNK : 0x0064CF10
void Sys_EnumerateHw()
{
    memory::call<void()>(0x0064CF10)();
}