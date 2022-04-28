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
bool R_PushRemoteScreenUpdate()
{
    int i; // edi
    bool IsMainThread; // al

    for (i = a1; i; --i)
    {
        IsMainThread = Sys_IsMainThread();
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
