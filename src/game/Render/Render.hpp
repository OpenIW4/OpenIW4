#pragma once

#include "../stdafx.hpp"

int R_PopRemoteScreenUpdate();
void R_EndRemoteScreenUpdate();
bool R_PushRemoteScreenUpdate(std::int32_t a1);
std::int32_t FixWindowsDesktop();