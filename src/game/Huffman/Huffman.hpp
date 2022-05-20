#pragma once

#include "../defs.hpp"

void Huff_Init(huffman_t* huff);
void Huff_BuildFromData(huff_t* huff, std::int32_t* msg);
nodetype* Huff_initNode(huff_t* huff, std::int32_t ch, std::int32_t weight);
std::int32_t nodeCmp(const void* left, const void* right);
