#include "Render.hpp"
#include "../Sys/Sys.hpp"

#include <utils/memory/memory.hpp>

//DONE : 0x0050B0D0
int R_PopRemoteScreenUpdate()
{
	int result; // esi

	result = *(int*)(0x066DAD64)/*r_glob*/;

	while (*(int*)(0x066DAD64)/*r_glob*/)
	{
		R_EndRemoteScreenUpdate();
	}

	return result;
}

//DONE : 0x0050AFE0
void R_EndRemoteScreenUpdate()
{
    if (Sys_IsMainThread() && *(bool*)(0x066DAD5D))
    {
        if (*(int*)(0x066DAD64)/*r_glob*/ == 1)
        {
            while (!*(int*)(0x066DAD60))
            {
                Sys_Sleep(1u);
            }

            *(int*)(0x066DAD60) = 0;
            *(int*)(0x066DAD64)/*r_glob*/ = *(int*)(0x066DAD64)/*r_glob*/ - 1;

            do
            {
                *(int*)(0x00134C8AC) = *(int*)(0x00134C8AC) + 1;
                Sys_Sleep(1u);
                *(int*)(0x00134C8AC) = *(int*)(0x00134C8AC) - 1;
            } while (!*(int*)(0x066DAD60));

            *(int*)(0x066DAD60) = 0;
        }
        else
        {
            *(int*)(0x066DAD64)/*r_glob*/ = *(int*)(0x066DAD64)/*r_glob*/ - 1;
        }
    }
}

//DONE : 0x0050B070
bool R_PushRemoteScreenUpdate(std::int32_t a1)
{
    int i; // edi
    bool IsMainThread = Sys_IsMainThread(); // al

    for (i = a1; i; --i)
    {
        if (IsMainThread)
        {
            if (*(bool*)(0x066DAD5D))
            {
                *(int*)(0x066DAD64) /*r_glob*/ = *(int*)(0x066DAD64) /*r_glob*/ + 1;
                *&IsMainThread = *(int*)(0x066DAD64) /*r_glob*/;
                if (!*&IsMainThread)
                {
                    if (*(bool*)(0x066DAD5D) && *(int*)(0x066DAD5C) /*r_glob_0*/)
                    {
                        Sys_ReleaseThreadOwnership();
                        *(int*)(0x066DAD5C) /*r_glob_0*/ = 0;
                    }
                    IsMainThread = memory::call<bool()>(0x04413B0)();
                }
            }
        }
    }
    return IsMainThread;
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
    unsigned __int16* v4;
    unsigned __int16 ramp[3][256];

    ChangeDisplaySettingsA(0, 0);
    unsigned long currentThreadId = GetCurrentThreadId();
    EnumThreadWindows(currentThreadId, HideWindowCallback, 0);
    HWND desktopWindow = GetDesktopWindow();
    HDC DC = GetDC(desktopWindow);

    std::int32_t v3 = 0;
    v4 = ramp[1];
    std::int32_t v5 = 256;

    do
    {
        *(v4 - 256) = v3;
        *v4 = v3;
        v4[256] = v3;
        v3 += 257;
        ++v4;
        --v5;
    } 
    while (v5);

    SetDeviceGammaRamp(DC, ramp);
    ReleaseDC(desktopWindow, DC);
}