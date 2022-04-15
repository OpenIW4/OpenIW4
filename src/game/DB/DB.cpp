#include "DB.hpp"
#include "../Sys/Sys.hpp"

#include <utils/memory/memory.hpp>

//DONE : 0x00440EC0
void DB_Update()
{
    if (!Sys_IsDatabaseReady2())
    {
        if (Sys_IsDatabaseReady())
        {
            DB_PostLoadXZone();
        }
    }
}

//THUNK : 0x005BC6A0
void DB_PostLoadXZone()
{
    memory::call<void()>(0x005BC6A0)();
}