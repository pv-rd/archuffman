#include "Node.hpp"

#include <iostream>
#include <cstdint>

#define SIZE 512
using index = std::uint16_t;
using big = std::uint32_t;
using byte = std::uint8_t;

HuffNode * new_node(index Pos, HuffNode * Left, HuffNode * Right, byte * code)
{
    if (Pos < 0 || Pos >= SIZE) return nullptr;
    return new HuffNode{Pos, Left, Right, code};
}

HuffNode * get_parent(HuffNode * Left, HuffNode * Right, big * table, index extra)
{
    if (Left == nullptr && Right == nullptr) return new_node(extra);

    if (Left == nullptr) table[extra] = table[Right->pos];
    else if (Right == nullptr) table[extra] = table[Left->pos];
    else table[extra] = table[Right->pos] + table[Left->pos];

    return new_node(extra, Left, Right);
}

big get_value(HuffNode * node, big * table)
{
    if (table == nullptr || node == nullptr) return 0;
    return table[node->pos];
}

HuffNode * erase_node(HuffNode * node)
{
    delete[] node->code;
    delete node;
    return nullptr;
}


