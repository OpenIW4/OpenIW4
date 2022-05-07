#pragma once

#include "../stdafx.hpp"

void MSG_WriteByte(unsigned long* a1, std::int8_t a2);
void MSG_WriteBit0(unsigned long* a1);
void MSG_WriteBit1(unsigned long* a1);
void MSG_WriteInt64(unsigned long* a1, std::int32_t a2, std::int32_t a3);
void MSG_WriteShort(unsigned long* a1, std::int16_t a2);
void MSG_WriteLong(unsigned long* a1, std::int32_t a2);