#pragma once

#include "../stdafx.hpp"
#include "../defs.hpp"

std::int32_t CL_ControllerIndexFromClientNum();
bool CL_AllLocalClientsInactive();
std::int32_t CL_GetFirstActiveControllerIndex();
void CL_ClearMutedList(SessionData* session);
