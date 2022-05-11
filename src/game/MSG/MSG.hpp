#pragma once

#include "../stdafx.hpp"
#include "../defs.hpp"

void MSG_Init(msg_t* buffer, const char* data, size_t size);
void MSG_WriteByte(msg_t* msg, std::int8_t c);
void MSG_WriteBit0(msg_t* msg);
void MSG_WriteBit1(msg_t* msg);
void MSG_WriteInt64(msg_t* msg, std::int32_t a2, std::int32_t a3);
void MSG_WriteShort(msg_t* msg, std::int32_t c);
void MSG_WriteLong(msg_t* msg, std::int32_t c);
void MSG_WriteData(msg_t* msg, std::int32_t* a2, std::int32_t a3);