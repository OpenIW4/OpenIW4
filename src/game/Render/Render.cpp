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