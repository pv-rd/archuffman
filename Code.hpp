#pragma once

#include <cstdint>
#include "Node.hpp"

bool get_bit(std::uint8_t Byte, int pos = 0);

std::uint8_t set_bit(std::uint8_t Byte, int pos, bool bit = true);

int byte_size(int bit_size);

void set_code(HuffNode * root, std::uint8_t * code = nullptr);
void print_code(std::uint8_t * code);
