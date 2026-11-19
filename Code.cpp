#include "Code.hpp"

#include <cstdint>
#include "Node.hpp"

#define SIZE 256

using byte = std::uint8_t;

bool get_bit(byte Byte, int pos)
{
    byte mask = 0b10000000 >> pos;
    return Byte & mask;
}

byte set_bit(byte Byte, int pos, bool st)
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

void set_code(HuffNode * root, byte * code)
{
    if (code == nullptr)
    {
        if (root->pos < SIZE)
        {
            code = new byte[2](); code[0] = 1; root->code = code; return;
        }
        code = new byte[1]; code[0] = 0;
    }

    root->code = code;

    if (root->left == nullptr && root->right == nullptr) return;

    byte bit_length =code[0];
    byte byte_length = byte_size(bit_length);
    byte child_length = byte_size(bit_length + 1);

    byte * left_code = new byte[child_length + 1]();
    byte * right_code = new byte[child_length + 1]();
    
    left_code[0] = bit_length+1; right_code[0] = bit_length + 1;

    for (int i = 1; i < byte_length + 1; i++)
    {
        left_code[i] = code[i]; right_code[i] = code[i];
    }

    right_code[child_length] = set_bit(right_code[child_length], (bit_length) % 8, true);

    if (root->left == nullptr)
    {
        set_code(root->right, right_code);
        delete [] left_code;
        return;
    }
    if (root->right == nullptr)
    {
        set_code(root->left, left_code);
        delete [] right_code;
        return;
    }
    set_code(root->right, right_code);
    set_code(root->left, left_code);
}

void print_code(byte * code)
{
    if (code[0] == 0) 
    {
        std::cout << "No code";
        return;
    }
    int length = byte_size(code[0]) + 1;
    for (int i = 1; i < length; i++) 
        {
            int stop = 8;
            if (code[0] - (i-1)*8 < 8) stop = code[0] - (i-1)*8;
            for (int j = 0; j < stop; j++) std::cout << (int)get_bit(code[i], j);
        }
    
}

