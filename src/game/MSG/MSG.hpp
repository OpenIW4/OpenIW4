#pragma once

#include "../stdafx.hpp"

DWORD* MSG_WriteByte(DWORD* a1, std::int8_t a2);
DWORD* MSG_WriteBit1(DWORD* a1);
void MSG_WriteInt64(DWORD* a1, std::int32_t a2, std::int32_t a3);