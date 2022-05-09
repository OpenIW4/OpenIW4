#pragma once

#include "../stdafx.hpp"
#include "../defs.hpp"

void MSG_WriteByte(msg_t* a1, std::int8_t a2);
void MSG_WriteBit0(msg_t* a1);
void MSG_WriteBit1(msg_t* a1);
void MSG_WriteInt64(msg_t* a1, std::int32_t a2, std::int32_t a3);
void MSG_WriteShort(msg_t* a1, std::uint16_t a2);
void MSG_WriteLong(msg_t* a1, std::int32_t a2);
void MSG_WriteData(msg_t* a1, std::int32_t* a2, std::int32_t a3);