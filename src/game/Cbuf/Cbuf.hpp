#pragma once

#include "../stdafx.hpp"

void Cbuf_AddText(int a1, const char* a2);
void Con_GetTextCopy(char* text, int maxSize);
int Conbuf_CleanText(const char* source, char* target, int sizeofTarget);
long Conbuf_AppendText(const char* a1);