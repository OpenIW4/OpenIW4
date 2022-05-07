#pragma once

#include "../stdafx.hpp"

unsigned long* MSG_WriteByte(unsigned long* a1, std::int8_t a2);
unsigned long* MSG_WriteBit1(unsigned long* a1);
void MSG_WriteInt64(DWORD* a1, std::int32_t a2, std::int32_t a3);
