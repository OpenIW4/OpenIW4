#pragma once

#include "../defs.hpp"

void Huff_Init(huffman_t* huff);
void Huff_BuildFromData(huff_t* huff, std::int32_t* msg);
