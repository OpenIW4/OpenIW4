#pragma once

#include "../stdafx.hpp"
#include "../defs.hpp"

#pragma region Session Variables
static SessionData session = *reinterpret_cast<SessionData*>(0x66B7008);
static SessionData g_partySession = *reinterpret_cast<SessionData*>(0x66B68B8);
static SessionData g_partyPresenceSession = *reinterpret_cast<SessionData*>(0x66B4E68);
static SessionData g_searchSession = *reinterpret_cast<SessionData*>(0x66B7758);

static SessionTaskData* sessionTaskData = *reinterpret_cast<SessionTaskData**>(0x66B55B8); // defined as sessionTaskData[32] in IDA, do not go above

static SessionModSkillData sessionModSkillData = *reinterpret_cast<SessionModSkillData*>(0x66B7EA8);

static const dvar_t* matchmaking_debug = *reinterpret_cast<dvar_t**>(0x66B4E60);
static const dvar_t* iwnet_sessionHeartBeat = *reinterpret_cast<dvar_t**>(0x66B902C);
static const dvar_t* iwnet_sessionFullHeartbeat = *reinterpret_cast<dvar_t**>(0x66B9028);
static const dvar_t* session_immediateDeleteTinySessions = *reinterpret_cast<dvar_t**>(0x66B4E64);
#pragma endregion Session Variables

void* Session_Init();
bool Session_IsUserRegistered(SessionData* session, const std::int32_t clientNum);
