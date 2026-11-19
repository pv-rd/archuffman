#pragma once

#include <iostream>
#include <cstdint>

struct HuffNode
{
    std::uint16_t pos;
    HuffNode * left;
    HuffNode * right;
    std::uint8_t * code;
};

HuffNode * new_node(std::uint16_t Pos, HuffNode * Left = nullptr, HuffNode * Right = nullptr, std::uint8_t * code = nullptr);
HuffNode * get_parent(HuffNode * Left, HuffNode * Right, std::uint32_t * table, std::uint16_t extra);
std::uint32_t get_value(HuffNode * node, std::uint32_t * table);
HuffNode * erase_node(HuffNode * node);
