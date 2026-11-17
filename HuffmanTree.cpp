#include "HuffmanTree.hpp"

#include <iostream>
#include <cstdint>

#define SIZE 512

using index = std::uint16_t;
using big = std::uint32_t;
using byte = std::uint8_t;

HuffNode * new_node(big * Ptr, HuffNode * Left, HuffNode * Right, byte * code)
{
    return new HuffNode{Ptr, Left, Right, code};
}

HuffNode * get_parent(HuffNode * Left, HuffNode * Right, uint32_t * table, uint16_t extra)
{
    if (Left == nullptr && Right == nullptr) return new_node(table + extra, nullptr, nullptr, nullptr);
    if (Left == nullptr) table[extra] = *(Right->ptr);
    if (Right == nullptr) table[extra] = *(Left->ptr);
    table[extra] = *(Left->ptr) + *(Right->ptr);
    return new_node(table + extra, Left, Right, nullptr);
}

