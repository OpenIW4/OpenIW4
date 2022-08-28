#include "Session.hpp"

//DONE : 0x44F1D0
bool Session_IsUserRegistered(SessionData* session, const std::int32_t clientNum)
{
    return session->dyn.users[clientNum].registered;
}
