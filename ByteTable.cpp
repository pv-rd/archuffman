#include "ByteTable.hpp"

#include <iostream>
#include <fstream>
#include <cstdint>

#define SIZE 512

using index = std::uint16_t;
using big = std::uint32_t;

big * new_table()
{
    return new big[SIZE]();
}

big * get_table(const char * file_name)
{
    std::ifstream file(file_name, std::ios::binary);
    if (!file.is_open()) return nullptr;
    char current;
    big * res = new_table();
    while (file.get(current)) res[(index)(unsigned char)current]++;  
    file.close();
    return res;
}

big get_size(const char * file_name)
{
    std::ifstream file(file_name, std::ios::binary|std::ios::ate);
    big res = file.tellg();
    file.close();
    return res;
}

int count_table(big * table)
{
    int res = 0;
    for (int i = 0; i < SIZE; i++) if (table[i] > 0) res++;
    return res;
}

void print_table(big * table, PrintMode pm)
{
    if (table == nullptr) std::cout << "No table\n";
    else
    {
        if (pm == PrintMode::Full)
        {
            for (int i = 0; i < SIZE; i++)
            {
                std::cout <<  i << "\t" << table[i] << '\n';
            }
        }
        else if (pm == PrintMode::Half)
        {
            for (int i = 0; i < SIZE/2; i++)
            {
                std::cout <<  i << "\t" << table[i] << '\n';
            }
        }
        else if (pm == PrintMode::Nonzero)
        {
            for (int i = 0; i < SIZE; i++)
            {
                if (table[i] > 0) std::cout <<  i << "\t" << table[i] << '\n';
            }
        }
        
    }
}

std::uint32_t * get_ptr(std::uint32_t * table, std::uint16_t index)
{
    return table + index;
}
