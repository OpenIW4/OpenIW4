#pragma once

#include "../stdafx.hpp"
#include "../defs.hpp"

static GfxGlobals r_glob = *reinterpret_cast<GfxGlobals*>(0x66DAD5C);
static bool byte_66E1E30 = *reinterpret_cast<bool*>(0x66E1E30); //this is most likely some bool value

std::int32_t R_PopRemoteScreenUpdate();
void R_EndRemoteScreenUpdate();
void R_SyncRenderThread();
