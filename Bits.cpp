#include "Bits.hpp"

#include <cstdint>

using byte = std::uint8_t;

bool get_bit(byte Byte, int pos)
{
    byte mask = 0b10000000 >> pos;
    return Byte & mask;
}

byte set_bit(byte Byte, int pos, bool st = true)
{
    byte mask = 0b10000000 >> pos;
    if (st)
        return Byte | mask;
    else
        return Byte & (~mask);
}

int byte_size(int bit_size)
{
    if (bit_size == 0) return 0;
    return (bit_size - 1) / 8 + 1;
}