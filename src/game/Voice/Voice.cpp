#include "Voice.hpp"

//DONE : 0x4C6B50
void Voice_UnmuteMember(SessionData* session, const std::int32_t clientNum)
{
    session->dyn.users[clientNum].muted = 0;
}
