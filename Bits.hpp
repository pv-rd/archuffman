#pragma once

#include <cstdint>

bool get_bit(std::uint8_t Byte, int pos = 0);

std::uint8_t set_bit(std::uint8_t Byte, int pos, bool bit = true);

int byte_size(int bit_size);
