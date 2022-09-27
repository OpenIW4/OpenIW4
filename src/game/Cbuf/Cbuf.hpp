#pragma once

#include "../stdafx.hpp"
static Console con = *reinterpret_cast<Console*>(0x9FDCF8);

void Cbuf_AddText(int a1, const char* a2);
void Con_GetTextCopy(char* text, int maxSize);
int Conbuf_CleanText(const char* source, char* target, int sizeofTarget);
void Conbuf_AppendText(const char* pMsg);
