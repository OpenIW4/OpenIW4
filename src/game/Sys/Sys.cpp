#include "Sys.hpp"

#include "../Com/Com.hpp"
#include "../Cbuf/Cbuf.hpp"
#include "../CL/CL.hpp"
#include "../NET/NET.hpp"
#include "../Win/Win.hpp"

#include <utils/memory/memory.hpp>

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

/*static _RTL_CRITICAL_SECTION** s_criticalSection = reinterpret_cast<_RTL_CRITICAL_SECTION**>(0x6499BC8);
static int* marker_win_configure = reinterpret_cast<int*>(0x6499F88);

//DONE : 0x42F0A0
void Sys_InitializeCriticalSections()
{
    _RTL_CRITICAL_SECTION* section = *s_criticalSection;
    do
        InitializeCriticalSection(section++);
    while ((int)section < (int)marker_win_configure);
}*/

//temp fix
//THUNK : 0x42F0A0
void Sys_InitializeCriticalSections()
{
    memory::call<void()>(0x42F0A0)();
}

//THUNK : 0x4301B0
void Sys_InitMainThread()
{
    memory::call<void()>(0x004301B0)();
}

//DONE : 0x4169C0
void Sys_Sleep(DWORD dwMilliseconds)
{
    Sleep(dwMilliseconds);
}

//THUNK but needs to move!
//TODO : 0x0064C620
bool IsServerRunning()
{
    return memory::call<bool()>(0x64C620)();
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

//THUNK : 0x4EC730
int* Sys_GetValue(int a1)
{
    return memory::call<int*(int)>(0x004EC730)(a1);
}

//THUNK : 0x4B2F50
void Sys_SetValue(int valueIndex, void* data)
{
    memory::call<void*(int, void*)>(0x4B2F50)(valueIndex, data);
}

//DONE : 0x64D300
void Sys_getcwd()
{
    char destBuf[256];

    _getcwd(destBuf, 256);
    Com_Printf(16, "Working directory: %s\n", destBuf);
}

//THUNK : 0x4C37D0
bool Sys_IsMainThread()
{
    return memory::call<bool()>(0x004C37D0)();
}

//THUNK : 0x4FC200
//game seems to crash here when using disconnect command, lets have the game handle it for now
void Sys_EnterCriticalSection(CriticalSection critSect)
{
    memory::call<void(CriticalSection)>(0x4FC200)(critSect);
}

//THUNK : 0x41B8C0
//see above
void Sys_LeaveCriticalSection(CriticalSection critSect)
{
    memory::call<void(CriticalSection)>(0x41B8C0)(critSect);
}

//TODO : 0x64CF10
void Sys_EnumerateHw()
{
    //memory::call<void()>(0x0064CF10)();
    //there is still a lot here to do
    //DX9 SDK will be needed quite soon
    *(std::int32_t*)0x64A17B0 = Sys_GetCPUCount();

    *(std::double_t*)0x64A17A0 = 1.0 / (((std::double_t)1i64 - (double)0i64) * *(std::double_t*)0x6499BA8 * 1000000.0);

    *(std::int64_t*)0x64A17B8 = Sys_SystemMemoryMB();
}

//DONE : 0x4C9540
void Sys_NormalExit()
{
    DeleteFileA((const char*)0x649FF74); //sys_processSemaphoreFile
}

//DONE : 0x4B14B0
std::int32_t Sys_SystemMemoryMB()
{
    HWND ActiveWindow;
    HWND v2;
    const char* v3;
    const char* v4;
    const char* v5;
    const char* v6;
    std::float_t v7, v8;
    std::int32_t sysMB, sysMBa;
    HINSTANCE__* hm;
    _MEMORYSTATUS status;
    int(__stdcall * MemStatEx)(_MEMORYSTATUSEX*);
    _MEMORYSTATUSEX statusEx;

    hm = GetModuleHandleA("kernel32.dll");
    if (hm && (MemStatEx = (int(__stdcall*)(_MEMORYSTATUSEX*))GetProcAddress(hm, "GlobalMemoryStatusEx")) != 0)
    {
        statusEx.dwLength = 64;
        MemStatEx(&statusEx);
        if (statusEx.ullAvailVirtual < 0x8000000)
        {
            v5 = Win_LocalizeRef("WIN_LOW_MEMORY_TITLE");
            v3 = Win_LocalizeRef("WIN_LOW_MEMORY_BODY");
            ActiveWindow = GetActiveWindow();
            if (MessageBoxA(ActiveWindow, v3, v5, 0x34u) != 6)
            {
                Sys_NormalExit();
                exit(0);
            }
        }
        v8 = (std::double_t)statusEx.ullTotalPhys * 0.00000095367432;
        sysMB = (std::int32_t)(v8 + 0.4999999990686774);
        if ((std::double_t)statusEx.ullTotalPhys > (std::double_t)sysMB * 1048576.0 || sysMB > 2048)
            return 2048;
        return sysMB;
    }
    else
    {
        status.dwLength = 32;
        GlobalMemoryStatus(&status);
        if (status.dwAvailVirtual < 0x8000000)
        {
            v6 = Win_LocalizeRef("WIN_LOW_MEMORY_TITLE");
            v4 = Win_LocalizeRef("WIN_LOW_MEMORY_BODY");
            v2 = GetActiveWindow();
            if (MessageBoxA(v2, v4, v6, 0x34u) != 6)
            {
                Sys_NormalExit();
                exit(0);
            }
        }
        v7 = (std::double_t)status.dwTotalPhys * 0.00000095367432;
        sysMBa = (std::int32_t)(v7 + 0.4999999990686774);
        if ((std::double_t)status.dwTotalPhys > (std::double_t)sysMBa * 1048576.0 || sysMBa > 2048)
            return 2048;
        return sysMBa;
    }
}

//DONE: 0x4C6610
std::int32_t Sys_GetCPUCount()
{
    return *(std::int32_t*)0x1CDE7F0; //s_cpuCount
}

//DONE : 0x4CA4A0
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
    Sys_EnterCriticalSection(CRITSECT_START_SERVER);
    *(DWORD*)0x1CDE84C = 1; //dword_1CDE84C
    Sys_LeaveCriticalSection(CRITSECT_START_SERVER);

    HANDLE result = *(HANDLE*)0x1CDE85C; //dword_1CDE85C

    if(result)
    {
        WaitForSingleObject(result, INFINITE);
    }

    HANDLE event = *(HANDLE*)0x1CDE7F8; //dword_1CDE7F8
    return SetEvent(event);
}

//TODO : 0x43D570
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
    I_strncpyz((char*)0x64A329C, text, 512);

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
    Sys_EnterCriticalSection(CRITSECT_FX_ALLOC);
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

//DONE : 0x4B20E0
bool Sys_IsRenderThread()
{
    return GetCurrentThreadId() == *(unsigned long*)0x1CDE800;
}

//DONE : 0x4B0270
bool Sys_IsServerThread()
{
    return GetCurrentThreadId() == *(unsigned long*)0x1CDE80C;
}

//DONE : 0x4C6020
bool Sys_IsDatabaseThread()
{
    return GetCurrentThreadId() == *(unsigned long*)0x1CDE814;
}


//DONE : 0x4B4B40
//may need to be moved
void NetadrToSockadr(netadr_t* a1, sockaddr* a2)
{
    netadrtype_t type = a1->type;
    *(unsigned long*)&a2->sa_family = 0;
    *(unsigned long*)&a2->sa_data[2] = 0;
    *(unsigned long*)&a2->sa_data[6] = 0;
    *(unsigned long*)&a2->sa_data[10] = 0;

    if (type == NA_BROADCAST)
    {
        a2->sa_family = 2;
        *(std::uint16_t*)a2->sa_data = a1->port;
        *(unsigned long*)&a2->sa_data[2] = -1;
    }
    else
    {
        switch (type)
        {
            case NA_IP:
                a2->sa_family = 2;
                *(unsigned long*)&a2->sa_data[2] = *(unsigned long*)a1->ip;
                *(std::uint16_t*)a2->sa_data = a1->port;
                break;
            case NA_IPX:
                a2->sa_family = 6;
                *(unsigned long*)&a2->sa_data = *(unsigned long*)a1->ipx;
                *(unsigned long*)&a2->sa_data[4] = *(unsigned long*)&a1->ipx[4];
                *(unsigned long*)&a2->sa_data[8] = *(std::uint16_t*)&a1->ipx[8];
                *(std::uint16_t*)&a2->sa_data[10] = a1->port;
                break;
            case NA_BROADCAST_IPX:
                a2->sa_family = 6;
                *(unsigned long*)a2->sa_data = 0;
                *(unsigned long*)&a2->sa_data[4] = -1;
                *(std::uint16_t*)&a2->sa_data[8] = -1;
                *(std::uint16_t*)&a2->sa_data[10] = a1->port;
                break;
        }
    }
}

char Sys_SendPacket(std::int32_t len, const void* data, netadr_t to)
{
    std::uint32_t v3; //SOCKET
    sockaddr toa;
    std::int32_t v5;

    switch (to.type)
    {
        case NA_BROADCAST:
        case NA_IP:
            v3 = *(unsigned long*)0x64A3008;
            break;
        case NA_BROADCAST_IPX:
        case NA_IPX:
            v3 = *(unsigned long*)0x64A1E1C;
            break;
        default:
            memory::call<void(std::int32_t, char*, netadrtype_t)>(0x4B22D0)(0, *(char**)0x70909C, to.type); //com_error
            return 1;
    }

    if (!v3)
    {
        return 1;
    }

    NetadrToSockadr(&to, &toa);

    if (*(unsigned long*)0x64A3018 && to.type == NA_IP)
    {
        *(char*)0x64A1E70 = 0; //buf
        *(char*)0x64A1E71 = 0;
        *(char*)0x64A1E72 = 0;
        *(char*)0x64A1E73 = 1;

        *(unsigned long*)0x64A1E74 = *(unsigned long*)&toa.sa_data[2];
        *(std::uint16_t*)0x64A1E78 = *(std::uint16_t*)toa.sa_data;
        memcpy(*(void**)0x64A1E7A, data, len);
        v5 = sendto(v3, *(const char**)0x64A1E70, len + 10, 0, *(sockaddr**)0x64A1E08, 16);
    }
    else
    {
        v5 = sendto(v3, (const char*)data, len, 0, &toa, 16);
    }

    if (v5 != -1)
    {
        return 1;
    }

    std::int32_t error = WSAGetLastError();

    if (error == 10035 || error == 10049 && (to.type == NA_BROADCAST || to.type == NA_BROADCAST_IPX))
    {
        return 1;
    }
    const char* v7 = NET_ErrorString();
    memory::call<std::int32_t(std::int32_t, char*, const char*)>(0x4F8C70)(16, "Sys_SendPacket: %s\n", v7); //com_error_0
    return 0;
}

volatile int* sv_thread_owns_game = reinterpret_cast<volatile int*>(0x2089DB8);

int SV_GetServerThreadOwnsGame()
{
    return *sv_thread_owns_game;
}
