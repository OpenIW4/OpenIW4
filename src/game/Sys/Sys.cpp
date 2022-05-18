#include "game/defs.hpp"

#include "Sys.hpp"

#include "../Com/Com.hpp"
#include "../Cbuf/Cbuf.hpp"
#include "../CL/CL.hpp"
#include "../main.hpp"

#include <memory/memory.hpp>

//DONE : 0x004305E0
void Sys_ShowConsole()
{
    HMODULE handle;
    if (!*(HWND*)0x064A3288)
    {
        handle = GetModuleHandleA(0);
        Sys_CreateConsole(handle);
    }

    ShowWindow(*(HWND*)0x064A3288 /*s_wcd.hWnd*/, 1);
    SendMessageA(*(HWND*)0x0064A328C /*s_wcd.hwndBuffer*/, 0x00B6, 0, 0xFFFF);
}

//DONE : 0x004288A0
void Sys_CreateConsole(HINSTANCE hInstance)
{
    HDC hDC;
    WNDCLASSA WndClass;
    tagRECT Rect;
    int nHeight;
    int swidth, sheight;

    char text[16384];
    char target[16384];

    WndClass.style = 0;
    WndClass.lpfnWndProc = ConsoleWndProc;
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hInstance = hInstance;
    WndClass.hIcon = LoadIconA(hInstance, (LPCSTR)1);
    WndClass.hCursor = LoadCursorA(0, (LPCSTR)0x7F00);
    WndClass.hbrBackground = (HBRUSH)5;
    WndClass.lpszMenuName = 0;
    WndClass.lpszClassName = "OpenIW4 WinConsole";

    if (!RegisterClassA(&WndClass))
    {
        return;
    }

    Rect.left = 0;
    Rect.right = 620;
    Rect.top = 0;
    Rect.bottom = 450;
    AdjustWindowRect(&Rect, 0x80CA0000, 0);

    hDC = GetDC(GetDesktopWindow());
    swidth = GetDeviceCaps(hDC, 8);
    sheight = GetDeviceCaps(hDC, 10);
    ReleaseDC(GetDesktopWindow(), hDC);

    *(int*)0x64A389C = Rect.right - Rect.left + 1;
    *(int*)0x64A38A0 = Rect.bottom - Rect.top + 1;

    *(HWND*)0x64A3288 = CreateWindowExA( // hWndParent
        0, "OpenIW4 WinConsole", "OpenIW4 Console", 0x80CA0000, (swidth - 600) / 2, (sheight - 450) / 2,
        Rect.right - Rect.left + 1, Rect.bottom - Rect.top + 1, 0, 0, hInstance, 0);

    if (!*(HWND*)0x64A3288)
    {
        return;
    }

    // create fonts
    hDC = GetDC(*(HWND*)0x64A3288);
    nHeight = MulDiv(8, GetDeviceCaps(hDC, 90), 72);

    *(HFONT*)0x64A3294 = CreateFontA( // hfBufferFont
        -nHeight, 0, 0, 0, 300, 0, 0, 0, 1u, 0, 0, 0, 0x31u, "Courier New");

    ReleaseDC(*(HWND*)0x64A3288, hDC);

    auto logo = LoadImageA(0, "openiw4-logo.bmp", 0, 0, 0, 0x10u);
    if (logo)
    {
        *(HWND*)0x64A3290 = CreateWindowExA(
            0, "Static", 0, 0x5000000Eu, 5, 5, 0, 0, *(HWND*)0x64A3288, (HMENU)1, hInstance, 0);
        SendMessageA(*(HWND*)0x64A3290, 0x172u, 0, (LPARAM)logo);
    }

    // create the input line
    *(HWND*)0x64A3298 = CreateWindowExA( // hwndInputLine
        0, "edit", 0, 0x50800080u, 6, 400, 608, 20, *(HWND*)0x64A3288, (HMENU)0x65, hInstance, 0);
    *(HWND*)0x64A328C = CreateWindowExA( // hwndBuffer
        0, "edit", 0, 0x50A00844u, 6, 70, 606, 324, *(HWND*)0x64A3288, (HMENU)0x64, hInstance, 0);
    SendMessageA(*(HWND*)0x64A328C, 0x30, *(WPARAM*)0x64A3294, 0);

    *(WNDPROC*)0x64A38A4 = (WNDPROC)SetWindowLongA( // SysInputLineWndProc
        *(HWND*)0x64A3298, -4, (long)InputLineWndProc);
    SendMessageA(*(HWND*)0x64A3298, 0x30, *(WPARAM*)0x64A3294, 0);

    SetFocus(*(HWND*)0x64A3298);
    Con_GetTextCopy(text, 0x4000);
    Conbuf_CleanText(text, target, 0x4000);
    SetWindowTextA(*(HWND*)0x64A328C, target);
}

//DONE : 0x0064DC50
long __stdcall ConsoleWndProc(HWND hWnd, std::uint32_t msg, std::uint32_t wParam, long lParam)
{
    switch (msg)
    {
        case WM_SIZE: // 5
            SetWindowPos(*(HWND*)0x64A328C /*dword_64A328C*/, 0, 5, 70, lParam - 15, HIWORD(lParam) - 100, 0);
            SetWindowPos(*(HWND*)0x64A3298 /*::hWnd*/, 0, 5, HIWORD(lParam) - 100 + 78, lParam - 15, 20, 0);
            *(std::uint16_t*)0x64A389C /*dword_64A389C*/ = (std::uint16_t)lParam;
            *(long*)0x64A38A0 /*dword_64A38A0*/ = HIWORD(lParam);
            return DefWindowProcA(hWnd, msg, wParam, lParam);
        case WM_ACTIVATE: // 6
            if (wParam)
            {
                SetFocus(*(HWND*)0x64A3298 /*::hWnd*/);
            }
            return DefWindowProcA(hWnd, msg, wParam, lParam);
        case WM_CLOSE: // 16
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProcA(hWnd, msg, wParam, lParam);
    }
}

//DONE : 0x00470190
long __stdcall InputLineWndProc(HWND hWnd, std::uint32_t uMsg, std::uint32_t wParam, long lParam)
{
    char displayBuffer[1024];
    char inputBuffer[1024];

    if (uMsg == 8) // WM_KILLFOCUS
    {
        if ((HWND)wParam == *(HWND*)0x64A3288)
        {
            SetFocus(hWnd);
            return 0;
        }
    }
    else if (uMsg == 258 && wParam == 13) // WM_CHAR
    {
        GetWindowTextA(*(HWND*)0x64A3298, inputBuffer, 1024);
        strncat((char*)0x64A349C, inputBuffer, 507 - strlen((char*)0x64A349C));
        strcat((char*)0x64A349C, "\n");
        SetWindowTextA(*(HWND*)0x64A3298, "");

        Com_sprintf(displayBuffer, 1024, "]%s\n", inputBuffer);
        memory::call<void(char*)>(0x4914B0)(displayBuffer); // Sys_Print
        return 0;
    }

    return CallWindowProcA(*(WNDPROC*)0x64A38A4, hWnd, uMsg, wParam, lParam);
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

//THUNK but needs to move!
//TODO : 0x0064C620
bool IsServerRunning()
{
    return memory::call<bool()>(0x0064C620)();
}

//DONE : 0x0043EBB0
void Sys_CheckQuitRequest()
{
    int FirstActiveLocalClient; // eax
    auto onlinegame = (char*)(0x00B2BB48);
    auto xblive_privatematch = (char*)(0x0649E714);
    auto dword_649FF68 = (int*)(0x649FF68);

    if (*(bool*)(0x0649FB61)/*g_quitRequested*/ && Sys_IsMainThread())
    {
        if ( onlinegame[16]
            && !xblive_privatematch[16]
            && IsServerRunning()
            && Sys_Milliseconds() - *(int*)(0x0649FF94) < dword_649FF68[4])
        {
            if (!*(bool*)(0x0649FB60)/*g_quitMigrationStarted*/)
            {
                *(bool*)(0x0649FB60)/*g_quitMigrationStarted*/ = true;
                FirstActiveLocalClient = CL_ControllerIndexFromClientNum();
                Cbuf_AddText(FirstActiveLocalClient, "hostmigration_start\n");
            }
        }
        else
        {
            *(int*)(0x00A7FE90)/*cls*/ = 1;
            Cbuf_AddText(0, "quit\n");
        }
    }
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
    WndClass.hInstance = *(HINSTANCE*)0x064A3AD4;
    WndClass.hIcon = LoadIconA(*(HINSTANCE*)0x064A3AD4, (LPCSTR)1);
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
                *(HINSTANCE*)0x064A3AD4,
                0);
            *(HWND*)0x064A3050 /*g_splash_wv*/ = Window;
            if (Window)
            {
                v4 = CreateWindowExA(0, "Static", 0, 0x5000000Eu, 0, 0, 320, 100, Window, 0, *(HINSTANCE*)0x064A3AD4, 0);
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
                    SetWindowPos(*(HWND*)0x064A3050 /*g_splash_wv*/, 0, Rect.left, Rect.top, Rect.right - Rect.left, Rect.bottom - Rect.top, 4u);
                }
            }
        }
    }
}

//DONE : 0x004A7B10
void Sys_ShowSplashWindow()
{
    if (*(HWND*)0x064A3050 /*g_splash_wv*/)
    {
        ShowWindow(*(HWND*)0x064A3050 /*g_splash_wv*/, 5);
        UpdateWindow(*(HWND*)0x064A3050 /*g_splash_wv*/);
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
    if (!*(int*)0x064A304C /*Sys_Milliseconds(void)::initialized*/)
    {
        *(int*)0x064A3034 /*sys_timeBase*/ = timeGetTime();
        *(int*)0x064A304C /*Sys_Milliseconds(void)::initialized*/ = 1;
    }
    return timeGetTime() - *(int*)0x064A3034 /*sys_timeBase*/;
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

//DONE : 0x004CA4A0
bool Sys_IsDatabaseReady()
{
    return WaitForSingleObject(*(HANDLE*)(0x01CDE7F8)/*databaseCompletedEvent*/, 0) == 0;
}

//DONE : 0x00441280
bool Sys_IsDatabaseReady2()
{
    return WaitForSingleObject(*(HANDLE*)(0x01CDE858)/*databaseCompletedEvent2*/, 0) == 0;
}

//DONE : 0x004F5250
bool Sys_ReleaseThreadOwnership()
{
    return SetEvent(*(HANDLE*)0x01CDE704 /*noThreadOwnershipEvent*/);
}

//DONE : 0x004C3650
bool Sys_DatabaseCompleted()
{
    Sys_EnterCriticalSection(12);
    *(DWORD*)0x1CDE84C = 1; //dword_1CDE84C
    Sys_LeaveCriticalSection(12);

    HANDLE result = *(HANDLE*)0x1CDE85C; //dword_1CDE85C

    if(result)
    {
        WaitForSingleObject(result, INFINITE);
    }

    HANDLE event = *(HANDLE*)0x1CDE7F8; //dword_1CDE7F8
    return SetEvent(event);
}

//DONE : 0x43D570
void Sys_Error(const char* error, ...)
{
    tagMSG msg;
    char buffer[4096];
    va_list args;

    va_start(args, error);

    memory::call<void()>(0x4B9660)(); // our current Com_EnterError code is questionable, lets just call the game for now
    memory::call<void()>(0x45A190)(); // possibly Sys_SuspendOtherThreads

    _vsnprintf(buffer, 4096, error, args);
    va_end(args);

    FixWindowsDesktop();
    if (Sys_IsMainThread())
    {
        Sys_ShowConsole();
        Conbuf_AppendText("\n\n");
        Conbuf_AppendText(buffer);
        Conbuf_AppendText("\n");
    }
    Sys_SetErrorText(buffer);
    if (Sys_IsMainThread() && GetMessageA(&msg, 0, 0, 0))
    {
        do
        {
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        } 
        while (GetMessageA(&msg, 0, 0, 0));
    }

    memory::call<void()>(0x48A4E0)(); // Steam_EmergencyShutdown? idk if thats the name but it's something related to steam though
    exit(0);
}

//DONE : 0x433940
void Sys_SetErrorText(const char* text)
{
    // s_wcd.errorString seems to be leftover from Quake for setting an old error prompt
    //I_strncpyz(*(char**)0x64A329C, text, 512);

    DestroyWindow(*(HWND*)0x64A3298 /*s_wcd.hwndInputLine*/);
    *(HWND*)0x64A3298 = 0;

    auto activeWindow = GetActiveWindow();
    MessageBoxA(activeWindow, text, "Error", 0x10u);
}

//TODO : 0x4B2E60
void Sys_OutOfMemErrorInternal(const char* filename, int line)
{
    memory::call<void(const char*, int)>(0x4B2E60)(filename, line);
}

//TODO : 0x45A190
unsigned long Sys_SuspendOtherThreads()
{
    Sys_EnterCriticalSection(34);
    memory::call<int(void)>(0x51CA20)(); // R_Cinematic_SuspendPlayback(), if you follow it, it calls bink stuff
    unsigned long result = GetCurrentThreadId();
    unsigned long v1 = result;

    for (std::uint32_t i = 0; i < 36; i++)
    {
        if (*(HANDLE*)(*(char**)0x1CDE828 + i))
        {
            result = *(unsigned long*)(*(char**)0x1CDE7FC + i);
            if (result)
            {
                if (result != v1)
                {
                    result = SuspendThread(*(HANDLE*)(*(char**)0x1CDE828 + i));
                }
            }
        }
    }

    return result;
}

//DONE : Inlined
long Sys_InterlockedDecrement(volatile long* target)
{
    return InterlockedDecrement(target);
}

//DONE : Inlined
long Sys_InterlockedIncrement(volatile long* target)
{
    return InterlockedIncrement(target);
}

//DONE : Inlined
long Sys_InterlockedCompareExchange(volatile long* target, long exChange, long comperand)
{
    return InterlockedCompareExchange(target, exChange, comperand);
}

//DONE : Inlined
long Sys_InterlockedExchange(volatile long* target, long value)
{
    return InterlockedExchange(target, value);
}

//DONE : 0x478680
void Sys_TempPriorityAtLeastNormalBegin(TempPriority* tempPriority)
{
    const auto hThread = GetCurrentThread();
    tempPriority->threadHandle = hThread;

    const auto priority = GetThreadPriority(hThread);
    tempPriority->oldPriority = priority;

    if (priority < 0)
    {
        SetThreadPriority(hThread, 0);
    }
}

//DONE : 0x4DCF00
void Sys_TempPriorityEnd(TempPriority* tempPriority)
{
    if (tempPriority->oldPriority < 0)
    {
        SetThreadPriority(tempPriority->threadHandle, tempPriority->oldPriority);
    }
}

//DONE : 0x435880
void Sys_LockWrite(FastCriticalSection* section)
{
    TempPriority temp;
    Sys_TempPriorityAtLeastNormalBegin(&temp);

    while (true)
    {
        if (section->readCount == 0)
        {
            if ((Sys_InterlockedIncrement(&section->writeCount) == 1) && (section->readCount == 0))
            {
                section->tempPriority.threadHandle = temp.threadHandle;
                section->tempPriority.oldPriority = temp.oldPriority;
                return;
            }

            Sys_InterlockedDecrement(&section->writeCount);
        }

        Sys_Sleep(1);
    }
}

//DONE : Inlined
void Sys_UnlockWrite(FastCriticalSection* section)
{
    Sys_InterlockedDecrement(&section->writeCount);
    Sys_TempPriorityEnd(&section->tempPriority);
}

//DONE : Inlined
void Sys_LockRead(FastCriticalSection* section)
{
    Sys_InterlockedIncrement(&section->readCount);
    while (section->writeCount)
    {
        Sys_Sleep(1);
    }
}

//DONE : Inlined
void Sys_UnlockRead(FastCriticalSection* section)
{
    Sys_InterlockedDecrement(&section->readCount);
}

//THUNK : 0x6417F0
int __stdcall HideWindowCallback(HWND hWnd, long lParam)
{
    int style;
    int styleEx;
    // int hiddenCount;
    char caption[1024];

    if (!GetWindowTextA(hWnd, caption, 1024) || !strcmp(caption, "Modern Warfare 2 Multiplayer")) // the build display name is a returned string from a function
    {
        style = GetWindowLongA(hWnd, -16);
        styleEx = GetWindowLongA(hWnd, -20);

        if (style & 0x10000000)
        {
            SetWindowLongA(hWnd, -16, style & 0xEFFFFFFF);
            SetWindowLongA(hWnd, -20, styleEx & 0xFFFFFFF7);
        }

        return 1;
    }

    return 0;
}

//DONE : 0x4CFA00
void FixWindowsDesktop()
{
    std::uint64_t currentThreadId = GetCurrentThreadId();
    HWND hwndDesktop = GetDesktopWindow();
    HDC hdc = GetDC(hwndDesktop);

    std::uint16_t ramp[3][256];

    ChangeDisplaySettingsA(0, 0);
    EnumThreadWindows(currentThreadId, HideWindowCallback, 0);

    for (std::uint16_t i = 0; i < 256; i++)
    {
        ramp[0][i] = 257 * i;
        ramp[1][i] = 257 * i;
        ramp[2][i] = 257 * i;
    }

    SetDeviceGammaRamp(hdc, ramp);
    ReleaseDC(hwndDesktop, hdc);
}
