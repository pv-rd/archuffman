#include <iostream>
#include <fstream>
#include <cstdint>
#include <cstddef>

std::size_t I(char symbol)
{
    return (std::size_t)(unsigned char)symbol;
}

char deI(std::size_t index)
{
    return (char)(unsigned char)index;
}

uint32_t * new_table()
{
    return new uint32_t[0b11111111]();
}

void print_table(uint32_t * table, bool without_empty = false)
{
    if (table == nullptr) std::cout << "No table\n";
    else
    {
        std::cout << "Symbol | Code | Quantity\n";
        for (std::size_t i = 0; i < 0b100000000; i++)
            if (!without_empty || table[i] != 0) 
                std::cout << deI(i) << "\t|\t" << i << "\t|\t" << table[i] << '\n';
    }
}

bool add(char symbol, uint32_t * table)
{
    if (table[I(symbol)] == UINT32_MAX) return false;
    table[I(symbol)]++;
    return true;
}

uint32_t * count_file(const char * file_name)
{
    std::ifstream file(file_name);
    if (!file.is_open()) return nullptr;
    char current;
    uint32_t * res = new_table();
    while (file.get(current)) res[I(current)]++;  
    file.close();
    return res;
}

int main () 
{
    print_table(count_file("test.txt"), true);
    return 0;
}


