#include "Session.hpp"
#include "../Dvar/Dvar.hpp"

#include <utils/memory/memory.hpp>

//DONE : 0x6809C0
void* Session_Init()
{
    memset(sessionTaskData, 0, sizeof(SessionTaskData) * 32);
    //sessionTaskData is an array with 32 elements
    //thanks to drgn for helping me figure that out

    memory::call<void(std::int32_t*, const char*)>(0x430890)(*(std::int32_t**)0x66B9030, "Session_Init"); //TaskManager_ConstructTaskSet


    session.staticData.sessionName = "gameSession";
    session.staticData.registerUsersWithVoice = 1;
    g_partySession.staticData.sessionName = "partySession";
    g_partySession.staticData.registerUsersWithVoice = 1;
    g_partyPresenceSession.staticData.sessionName = "party presence session";
    g_partyPresenceSession.staticData.registerUsersWithVoice = 0;
    g_searchSession.staticData.sessionName = "searchSession";
    g_searchSession.staticData.registerUsersWithVoice = 0;

    matchmaking_debug = Dvar_RegisterBool("matchmaking_debug", 1, 0, "Enable matchmaking debug information");
    iwnet_sessionHeartBeat = Dvar_RegisterInt("iwnet_sessionHeartBeat", 5000, 3000, INT_MAX, 0, "How often to send a heartbeat msg");
    iwnet_sessionFullHeartbeat = Dvar_RegisterInt("iwnet_sessionFullHeartBeat", 20000, 3000, INT_MAX, 0, "How often to send a heartbeat msg if the game is full");
    session_immediateDeleteTinySessions = Dvar_RegisterBool("session_immediateDeleteTinySessions", 0, 0, "Whether to immediately delete sessions with 1 user");

    sessionModSkillData.needsUpdate = 0;
    sessionModSkillData.active = 0;
    return memset(sessionModSkillData.playerList, 204, sizeof(sessionModSkillData.playerList));
}

//DONE : 0x44F1D0
bool Session_IsUserRegistered(SessionData* session, const std::int32_t clientNum)
{
    return session->dyn.users[clientNum].registered;
}
