#pragma once

#include "../stdafx.hpp"
#include "../defs.hpp"

#pragma region MSG Variables
//unknown, seems to be referenced a lot
static msg_t unk_66C7160 = *reinterpret_cast<msg_t*>(0x66C7160);
static huffman_t* huff = reinterpret_cast<huffman_t*>(0x1CB9EC0);
static std::int32_t* msg = reinterpret_cast<std::int32_t*>(0x1CB9EB8);

static bool msgInit = *reinterpret_cast<bool*>(0x1CB9EB8);
#pragma endregion MSG Variables

void MSG_Init(msg_t* buffer, char* data, std::size_t size);
void MSG_WriteByte(msg_t* msg, std::int8_t c);
void MSG_WriteBit0(msg_t* msg);
void MSG_WriteBit1(msg_t* msg);
void MSG_WriteInt64(msg_t* msg, std::int32_t a2, std::int32_t a3);
void MSG_WriteShort(msg_t* msg, std::int32_t c);
void MSG_WriteLong(msg_t* msg, std::int32_t c);
void MSG_WriteData(msg_t* msg, std::int32_t* a2, std::int32_t a3);
void MSG_InitHuffman();
void MSG_Discard(msg_t* msg);
void MSG_ClearLastReferencedEntity(msg_t* msg);
char* MSG_ReadString(msg_t* msg, char* string, std::uint32_t maxChars);
std::int32_t MSG_ReadByte(msg_t* msg);
std::int32_t MSG_GetByte(msg_t* msg, std::int32_t where);
void MSG_WriteString(msg_t* msg, const char* source);
