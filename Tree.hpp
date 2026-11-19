#pragma once

#include <iostream>
#include <cstdint>
#include "Node.hpp"
#include "Heap.hpp"
#include "Code.hpp"

HuffNode * get_tree(HuffNode ** heap, std::uint32_t * table);

void print_tree(HuffNode * root, bool with_code = false, int align = 0);

HuffNode * erase_tree(HuffNode * root);
