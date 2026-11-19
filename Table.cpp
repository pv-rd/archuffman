#include "Table.hpp"

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

big * get_table(const char * file_name, bool count_size)
{
    std::ifstream file(file_name, std::ios::binary);
    if (!file.is_open()) return nullptr;
    char current;
    big * res = new_table();
    while (file.get(current)) 
    {
        res[(index)(unsigned char)current]++;
        if (count_size) res[SIZE]++;
    }
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

int count_nonzero(big * table)
{
    int res = 0;
    for (int i = 0; i < SIZE; i++) if (table[i] > 0) res++;
    return res;
}

void print_table(big * table, PrintMode mode)
{
    if (table == nullptr) std::cout << "No table\n";
    else
    {
        if (mode == PrintMode::Full)
        {
            for (int i = 0; i < SIZE; i++)
            {
                std::cout <<  i << "\t" << table[i] << '\n';
            }
        }
        else if (mode == PrintMode::Half)
        {
            for (int i = 0; i < SIZE/2; i++)
            {
                std::cout <<  i << "\t" << table[i] << '\n';
            }
        }
        else if (mode == PrintMode::Nonzero)
        {
            for (int i = 0; i < SIZE; i++)
            {
                if (table[i] > 0) std::cout <<  i << "\t" << table[i] << '\n';
            }
        }
        
    }
}

void erase_table(big * table)
{
    delete[] table;
}