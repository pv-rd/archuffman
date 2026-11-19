#pragma once

#include <iostream>
#include <cstdint>
#include "Node.hpp"
#include "Heap.hpp"
#include "Code.hpp"
#include "Tree.hpp"

void set_code_tree(HuffNode * root, std::uint8_t ** tree, int i=0);

std::uint8_t ** new_code_tree();

void print_code_tree(std::uint8_t ** tree);