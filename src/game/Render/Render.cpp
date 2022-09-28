#include "Render.hpp"
#include "../Sys/Sys.hpp"

#include <utils/memory/memory.hpp>

//DONE : 0x0050B0D0
int R_PopRemoteScreenUpdate()
{
    volatile std::int32_t remoteScreenUpdateNesting = r_glob.remoteScreenUpdateNesting;

	while (r_glob.remoteScreenUpdateNesting)
	{
		R_EndRemoteScreenUpdate();
	}

	return remoteScreenUpdateNesting;
}

//DONE : 0x0050AFE0
void R_EndRemoteScreenUpdate()
{
    if (Sys_IsMainThread() && r_glob.startedRenderThread)
    {
        if (r_glob.remoteScreenUpdateNesting)
        {
            while (!r_glob.screenUpdateNotify)
            {
                Sys_Sleep(1u);
            }

            r_glob.screenUpdateNotify = 0;
            --r_glob.remoteScreenUpdateNesting;

            do
            {
                *(int*)(0x00134C8AC) = *(int*)(0x00134C8AC) + 1;
                Sys_Sleep(1u);
                *(int*)(0x00134C8AC) = *(int*)(0x00134C8AC) - 1;
            } while (!r_glob.screenUpdateNotify);

            r_glob.screenUpdateNotify = 0;
        }
        else
        {
            --r_glob.remoteScreenUpdateNesting;
        }
    }
}
