#include "Sys.hpp"
#include "../Com/Com.hpp"
#include "../Cbuf/Cbuf.hpp"
#include "../CL/CL.hpp"

#include <utils/memory/memory.hpp>

#define EDIT_ID			100
#define INPUT_ID		101

//DONE : 0x004305E0
void Sys_ShowConsole()
{
    HMODULE handle;
    if (!*(HWND*)(0x064A3288))
    {
        handle = GetModuleHandleA(0);
        Sys_CreateConsole(handle);
    }

    // seems to match case 1 of Quake code (https://github.com/id-Software/Quake-III-Arena/blob/master/code/win32/win_syscon.c#L464)
    ShowWindow(*(HWND*)(0x064A3288) /*s_wcd.hWnd*/, SW_SHOWNORMAL);
    SendMessageA(*(HWND*)(0x0064A328C) /*s_wcd.hwndBuffer*/, EM_LINESCROLL, 0, 0xFFFF);
}

//DONE : 0x004288A0
void Sys_CreateConsole(HINSTANCE hInstance)
{
    // memory::call<void(HINSTANCE)>(0x004288A0)(hInstance);

    HDC hDC;
    HDC v5;
    std::int32_t v6, v7;
    HANDLE ImageA;
    RECT rect;
    WNDCLASS wc;

    int nHeight;
    int swidth, sheight;

    std::int8_t String[16384];

    const char* DEDCLASS = "OpenIW4 WinConsole";
    int DEDSTYLE = WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX;

    wc.style = 0;
    wc.lpfnWndProc = ConsoleWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(hInstance, (LPCSTR)1);
    wc.hCursor = LoadCursor(0, (LPCSTR)0x7F00);
    wc.hbrBackground = (HBRUSH)5;
    wc.lpszMenuName = 0;
    wc.lpszClassName = DEDCLASS;

    if (!RegisterClassA(&WndClass))
        return;

    rect.left = 0;
    rect.right = 620;
    rect.top = 0;
    rect.bottom = 450;
    AdjustWindowRect(&rect, DEDSTYLE, FALSE);

    hDC = GetDC(GetDesktopWindow());
    swidth = GetDeviceCaps(hDC, HORZRES);
    sheight = GetDeviceCaps(hDC, VERTRES);
    ReleaseDC(GetDesktopWindow(), hDC);

    *(int*)0x64A389C /*s_wcd.windowWidth*/ = rect.right - rect.left + 1;
    *(int*)0x64A38A0 /*s_wcd.windowHeight*/ = rect.bottom - rect.top + 1;

    *(HWND*)0x64A3288 /*s_wcd.hWnd*/ = CreateWindowEx(
        0,
        DEDCLASS,
        "OpenIW4 Console",
        DEDSTYLE,
        (swidth - 600) / 2,
        (sheight - 450) / 2,
        rect.right - rect.left + 1,
        rect.bottom - rect.top + 1,
        NULL,
        NULL,
        hInstance,
        NULL);

    if (*(HWND*)0x64A3288 /*s_wcd.hWnd*/ == NULL)
        return;
    
    // create fonts
    hDC = GetDC(*(HWND*)0x64A3288);
    nHeight = -MulDiv(8, GetDeviceCaps(hDC, LOGPIXELSY), 72);

    *(WPARAM*)0x64A3294 /*s_wcd.hfBufferFont*/ = (WPARAM)CreateFont(nHeight, 
        0, 0, 0, FW_LIGHT, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
        CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_MODERN | FIXED_PITCH, "Courier New");

    ReleaseDC(*(HWND*)0x64A3288 /*s_wcd.hWnd*/, hDC);
    logo = LoadImageA(0, "openiw4-logo.bmp", 0, 0, 0, 16);

    if (logo)
    {
        *(std::int32_t*)0x64A3290 = (std::int32_t)CreateWindowEx(
            0,
            "Static",
            0,
            0x5000000E,
            5,
            5,
            0,
            0,
            *(HWND*)0x64A3288,
            (HMENU)1,
            hInstance,
            0);

        SendMessage(*(HWND*)0x64A3290, 370, 0, (LPARAM)logo);
    }

    // create the input line
    *(HWND*)0x64A3298 /*s_wcd.hwndInputLine*/ = CreateWindow(
        "edit",
        NULL,
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
        6,
        400,
        608,
        20,
        *(HWND*)0x64A3288 /*s_wcd.hWnd*/,
        (HMENU)INPUT_ID,
        hInstance,
        NULL);

    // create the scrollbuffer
    *(HWND*)0x64A328C /*s_wcd.hwndBuffer*/ = CreateWindow(
        "edit",
        NULL,
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_BORDER | 
			ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
        6,
        70,
        606,
        324,
        *(HWND*)0x64A3288 /*s_wcd.hWnd*/,
        (HMENU)EDIT_ID,
        hInstance,
        NULL);

    SendMessage(*(HWND*)0x64A328C /*s_wcd.hwndBuffer*/, WM_SETFONT, *(WPARAM*)0x64A3294 /*s_wcd.hfBufferFont*/, 0);
    
    *(WNDPROC*)0x64A38A4 /*s_wcd.SysInputLineWndProc*/ = (WNDPROC)SetWindowLong(*(HWND*)0x64A3298 /*s_wcd.hwndInputLine*/, GWL_WNDPROC, (long)InputLineWndProc);
    SendMessage(*(HWND*)0x64A3298 /*s_wcd.hwndInputLine*/, WM_SETFONT, *(WPARAM*)0x64A3294 /*s_wcd.hfBufferFont*/, 0);
    
    SetFocus(*(HWND*)0x64A3298 /*s_wcd.hwndInputLine*/);
    memory::call<void* (void*, int)>(0x4AFB80)(v13, 0x4000);
    memory::call<std::uint8_t*(std::uint8_t*, std::int32_t, std::int8_t*)>(0x64DD30)((std::uint8_t*)v13, 0x4000, String);
    SetWindowTextA(*(HWND*)0x64A328C /*s_wcd.hwndBuffer*/, (LPCSTR)String);
}

//DONE : 0x0064DC50
LRESULT __stdcall ConsoleWndProc(HWND hWnd, std::uint32_t msg, std::uint32_t wParam, long lParam)
{
    std::uint32_t v4 = msg;

    switch (msg)
    {
        case 5:
            SetWindowPos(*(HWND*)0x64A328C, 0, 5, 70, lParam - 15, HIWORD(lParam) - 100, 0);
            SetWindowPos(*(HWND*)0x64A3298, 0, 5, HIWORD(lParam) - 100 + 78, lParam - 15, 20, 0);
            *(std::uint16_t*)0x64A389C = (std::uint16_t)lParam;
            *(long*)0x64A38A0 = HIWORD(lParam);
            v4 = msg;
            return DefWindowProcA(hWnd, v4, wParam, lParam);
        case 6:
            if (wParam)
            {
                SetFocus(*(HWND*)0x64A3298);
            }
            return DefWindowProcA(hWnd, v4, wParam, lParam);
        case 16:
            PostQuitMessage(0);
            return 0;

        default:
            return DefWindowProcA(hWnd, v4, wParam, lParam);
    }
}

//TODO : 0x00470190
// compared to quake https://github.com/id-Software/Quake-III-Arena/blob/master/code/win32/win_syscon.c#L260
LRESULT __stdcall InputLineWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    char inputBuffer[1024];
    char buffer[1024];

    switch (msg)
    {
        case WM_KILLFOCUS: // 8
            if ((HWND)wParam == *(HWND*)0x64A3288 /*s_wcd.hWnd*/)
            {
                SetFocus(hWnd);
                return 0;
            }
        case WM_CHAR: // 258
            if (wParam == 13)
            {
                GetWindowTextA(*(HWND*)0x64A3298 /*s_wcd.hwndInputLine*/, inputBuffer, sizeof(inputBuffer));
                strncat((char*)0x64A349C /*s_wcd.consoleText*/, inputBuffer, 507 - strlen((const char*)0x64A349C /*s_wcd.consoleText*/));
                *(WORD*)((char*)0x64A349C /*s_wcd.consoleText*/ + strlen(*(const char**)0x64A349C) /*s_wcd.consoleText*/) = 10;
                SetWindowText(*(HWND*)0x64A3298 /*s_wcd.hwndInputLine*/, *(LPCSTR*)0x6FAC0D);
                Com_sprintf(buffer, 1024, "]%s\n", inputBuffer);
                memory::call<void(char*)>(0x4914B0)(buffer);
            }
        break;
    }

    return CallWindowProcA(*(WNDPROC*)0x64A38A4 /*s_wcd.SysInputLineWndPro*/, hWnd, msg, wParam, lParam);
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

//THUNK : 0x0043D570
void Sys_Error(char* Format, ...)
{
    va_list args;
    memory::call<void(char*, va_list)>(0x0043D570)(Format, args);
}

//DONE : 0x004F5250
bool Sys_ReleaseThreadOwnership()
{
    return SetEvent(*(HANDLE*)(0x01CDE704) /*noThreadOwnershipEvent*/);
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