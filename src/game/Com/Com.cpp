#include "Com.hpp"
#include "../Sys/Sys.hpp"
#include "../Render/Render.hpp"
#include "../DB/DB.hpp"

#include <utils/memory/memory.hpp>

//THUNK : 0x004FF220
void Com_InitParse()
{
	memory::call<void()>(0x004FF220)();
}

//DONE : 0x004B7230
void Com_Frame()
{
    int* Value; // eax
    int v1; // esi

    Value = Sys_GetValue(2);

    if (!_setjmp(Value))
    {
        DB_Update();
        Com_Frame_Try_Block_Function();
        *(int*)(0x01AD8F80) = *(int*)(0x01AD8F80) + 1 /*com_frameNumber*/;
    }

    if (*(int*)(0x01AD8F1C)/*com_errorEnteredCount*/)
    {
        if (Sys_IsMainThread())
        {
            *(int*)(0x001AD8F2C) = R_PopRemoteScreenUpdate();
        }

        Sys_EnterCriticalSection(2);

        if (Sys_IsMainThread())
        {
            Com_EnterError();
        }

        v1 = *(int*)(0x01AD8F1C)/*com_errorEnteredCount*/;
        Sys_LeaveCriticalSection(2);

        if (Sys_IsMainThread())
        {
            Com_LeaveError();
        }

        if (v1)
        {
            Com_ErrorCleanup();
            Com_StartHunkUsers();
        }
    }
}

//THUNK : 0x004D9640
void Com_Init(char* src)
{
	memory::call<void(char*)>(0x004D9640)(src);
}

//THUNK : 0x0047DCA0
void Com_Frame_Try_Block_Function()
{
    memory::call<void()>(0x0047DCA0)();
}

//THUNK : 0x005091E0
bool Com_EnterError()
{
    return memory::call<bool()>(0x005091E0)();
}

//THUNK : 0x0050B070
int Com_LeaveError()
{
    return memory::call<int()>(0x0050B070)();
}

//THUNK : 0x0060BFD0
int Com_ErrorCleanup()
{
    return memory::call<int()>(0x0060BFD0)();
}

//THUNK : 0x0060BF40
int Com_StartHunkUsers()
{
    return memory::call<int()>(0x0060BF40)();
}