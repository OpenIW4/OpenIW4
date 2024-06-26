#pragma once

#include "../stdafx.hpp"
#include "../defs.hpp"

static SessionData* g_serverSession = reinterpret_cast<SessionData*>(0x66B7008);

SessionData* Live_GetCurrentSession();
bool Live_IsSignedIn();
std::int32_t Live_GetLSPXuid(std::int32_t a1, std::int64_t* a2);
const char* Live_GetLocalClientName();
