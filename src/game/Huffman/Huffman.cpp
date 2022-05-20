#include "Huffman.hpp"
#include "../Com/Com.hpp"

#include <memory/memory.hpp>

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

//DONE : Possibly inlined
nodetype* Huff_initNode(huff_t* huff, std::int32_t ch, std::int32_t weight)
{
    nodetype* tnode = &huff->nodeList[huff->blocNode++];
    tnode->symbol = ch;
    tnode->weight = weight;
    tnode->left = 0;
    tnode->right = 0;
    tnode->parent = 0;
    huff->loc[ch] = tnode;
    return tnode;
}

//DONE : 0x429170
std::int32_t nodeCmp(const void* left, const void* right)
{
    return *(unsigned long*)(*(unsigned long*)left + 12) - *(unsigned long*)(*(unsigned long*)right + 12);
}

void Huff_BuildFromData(huff_t* huff, std::int32_t* msg)
{
    nodetype* inited;
    nodetype* heap[256];
    nodetype* v4;

    std::int32_t numNodes = 256;
    std::int32_t heapHead = 0;

    for (std::int32_t i = 0; i < 256; ++i)
    {
        inited = Huff_initNode(huff, i, msg[i]);
        heap[i] = inited;
    }
    qsort(&heap, 0x100, 4, nodeCmp);
    nodetype* v3 = Huff_initNode(huff, 257, 1);
    v3->left = huff->tree;
    v3->right = heap[0];
    v3->left->parent = v3;
    v3->right->parent = v3;
    v3->weight = v3->right->weight + v3->left->weight;
    heap[0] = 0;

    while (numNodes > 1)
    {
        qsort(&heap[heapHead], 256 - heapHead, 4, nodeCmp);
        v4 = Huff_initNode(huff, 257, 1);
        v4->left = heap[heapHead];
        v4->right = heap[heapHead + 1];
        v4->left->parent = v4;
        v4->right->parent = v4;
        v4->weight = v4->right->weight + v4->left->weight;
        heap[++heapHead] = v4;
        --numNodes;
    }
    huff->tree = heap[heapHead];
}
