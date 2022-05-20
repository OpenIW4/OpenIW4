#include "Huffman.hpp"
#include "../Com/Com.hpp"


//DONE : 0x429270
void Huff_Init(huffman_t* huff)
{
    Com_Memset(&huff->compressDecompress.blocNode, 0, 19476);
    nodetype *node = &huff->compressDecompress.nodeList[huff->compressDecompress.blocNode++];
    huff->compressDecompress.loc[256] = node;
    huff->compressDecompress.tree = node;
    node->symbol = 256; //possibly a named constant
    huff->compressDecompress.tree->weight = 0;
    huff->compressDecompress.tree->parent = 0;
    huff->compressDecompress.tree->left = 0;
    huff->compressDecompress.tree->right = 0;
}


void Huff_BuildFromData(huff_t* huff, std::int32_t* msg)
{
    
}
