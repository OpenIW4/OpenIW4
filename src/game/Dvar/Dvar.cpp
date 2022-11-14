#include "Dvar.hpp"
#include "../Sys/Sys.hpp"
#include "../Unsorted/Unsorted.hpp"
#include "../Com/Com.hpp"
#include "../Script/StringList.hpp"

#include <utils/memory/memory.hpp>

//THUNK : 0x004D8220
void Dvar_Init()
{
	memory::call<void()>(0x004D8220)();
}

//THUNK : 0x00648440
const dvar_t* Dvar_RegisterFloat(char* command_name, float default_value, float minimum_value, float maximum_value, int flags, char* command_description)
{
	return memory::call<dvar_t*(char*, float, float, float, int, char*)>(0x00648440)
		(command_name, default_value, minimum_value, maximum_value, flags, command_description);
}

//THUNK : 0x004CE1A0
const dvar_t* Dvar_RegisterBool(const char* command_name, bool default_value, unsigned int maximum_value, const char* command_description)
{
	return memory::call<dvar_t*(const char*, bool, unsigned int, const char*)>(0x00648440)
		(command_name, default_value, maximum_value, command_description);
}

//THUNK : 0x479830
const dvar_t* Dvar_RegisterInt(const char* dvarName, std::int32_t a2, std::int32_t a3, std::int32_t a4, std::int32_t a5, const char* description)
{
    return memory::call<dvar_t*(const char*, std::int32_t, std::int32_t, std::int32_t, std::int32_t, const char*)>(0x479830)(dvarName, a2, a3, a4, a5, description);
}

//DONE : 0x6479C0
dvar_t* Dvar_FindMalleableVar(const char* dvarName)
{
    InterlockedIncrement(&dvarCritSect.readCount);

    while (dvarCritSect.writeCount)
    {
        Sys_Sleep(1);
    }

    dvar_t* v3 = dvarHashTable[generateHashValue(dvarName)];

    if (v3)
    {
        while (I_stricmp(dvarName, v3->name))
        {
            v3 = v3->hashNext;
            if (!v3)
            {
                InterlockedDecrement(&dvarCritSect.readCount);
                return 0;
            }
        }

        InterlockedDecrement(&dvarCritSect.readCount);
        return v3;
    }
    else
    {
        InterlockedDecrement(&dvarCritSect.readCount);
        return 0;
    }
}


//DONE : 0x4F3C80
const char* Dvar_AllocNameString(const char* name)
{
    //seems to be inlined
    return CopyString(name);
}

//there was more work here, but it was bad (awful)
