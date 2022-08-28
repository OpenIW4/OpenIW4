#include "Unsorted.hpp"
#include "../Com/Com.hpp"

#include <utils/memory/memory.hpp>

//check header for description


char* getBuildNumber()
{
    char* buffer = *(char**)0x7014640;
    sprintf(buffer, "%s %s %s %s", "177", "latest", __DATE__, __TIME__);
    return buffer;
}

//DONE : 0x646540
std::int32_t generateHashValue(const char* dvarName)
{
    //our com_error implementation is eh, use the game's for now
    //also this is really bad and needs to be rewritten, but this is temporary
    //*forgets to rewrite this*

    const char* v1 = dvarName;
    const char* v2 = v1;
    char v3 = *v2;
    std::uint16_t v4 = 0;

    if (!v1)
    {
        memory::call<void(errorParm_t, char*)>(0x4B22D0)(ERR_DROP, *(char**)0x6FABB0);
    }

    if (*v2)
    {
        std::uint16_t v5 = 0x77 - (std::uint16_t)v2;

        do
        {
            std::uint16_t v6 = (v5 + (std::uint16_t)v2++) * tolower(v3);
            v4 += v6;
            v3 = *v2;
        }
        while (*v2);
    }

    return v4 & 0x3FF;
}
