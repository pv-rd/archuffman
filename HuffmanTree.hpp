#pragma once

#include <iostream>
#include <cstdint>
#include "Bits.hpp"

struct HuffNode
{
    std::uint32_t * ptr;
    HuffNode * left;
    HuffNode * right;
    std::uint8_t * code;
};

HuffNode * new_node(std::uint32_t * Ptr, HuffNode * Left = nullptr, HuffNode * Right = nullptr, std::uint8_t * code = nullptr);
HuffNode * get_parent(HuffNode * Left, HuffNode * Right, std::uint32_t * table, std::uint16_t extra);




bool linearize(HuffNode * root, std::uint8_t ** array, std::uint16_t i, std::uint32_t * table);