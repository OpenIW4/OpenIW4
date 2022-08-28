#include "CL.hpp"
#include "../Voice/Voice.hpp"
#include "../Session/Session.hpp"

#include <utils/memory/memory.hpp>

//DONE : 0x00449E30
int CL_ControllerIndexFromClientNum()
{
	return 0;
}

//DONE : 0x4C10A0
bool CL_AllLocalClientsInactive()
{
    return false;
}

//DONE : 0x427A40
std::int32_t CL_GetFirstActiveControllerIndex()
{
    return 0;
}

//DONE : 0x46B6E0
void CL_ClearMutedList(SessionData* session)
{
    //18 is most likely the hard coded client limit
    //maybe one day have it as a macro
    for (std::int32_t i = 0; i < 18; ++i)
    {
        if (Session_IsUserRegistered(session, i))
        {
            Voice_UnmuteMember(session, i);
        }
    }
}
