#pragma once

#include <iostream>
#include <cstdint>
#include "Node.hpp"

int rise(HuffNode ** heap, int i, std::uint32_t * table);
int down(HuffNode ** heap, int i, std::uint32_t * table);
int add(HuffNode ** heap, HuffNode * node, int end, std::uint32_t * table);
HuffNode * del(HuffNode ** heap, int end, std::uint32_t * table);

void print_heap(HuffNode ** heap, std::uint32_t * table);

HuffNode ** get_heap(std::uint32_t * table);

int get_size(HuffNode ** heap);