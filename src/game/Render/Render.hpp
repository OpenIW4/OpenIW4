#pragma once

#include "../stdafx.hpp"
#include "../defs.hpp"

static GfxGlobals r_glob = *reinterpret_cast<GfxGlobals*>(0x66DAD5C);

int R_PopRemoteScreenUpdate();
void R_EndRemoteScreenUpdate();
