#include <iostream>
#include <fstream>
#include <cstdint>
#include <cstddef>

uint8_t I(char symbol)
{
    return (uint8_t)(unsigned char)symbol;
}

char deI(uint8_t index)
{
    return (char)(unsigned char)index;
}

uint64_t * new_table()
{
    return new uint64_t[0b100000000]();
}

void print_table(uint64_t * table, bool without_empty = false)
{
    if (table == nullptr) std::cout << "No table\n";
    else
    {
        std::cout << "Symbol | Code | Quantity\n";
        for (int i = 0; i <= 0b11111111; i++)
            if (!without_empty || table[i] != 0) 
                std::cout << deI(i) << "\t|\t" << i << "\t|\t" << table[i] << '\n';
    }
}

bool add(char symbol, uint64_t * table)
{
    if (table[I(symbol)] == UINT64_MAX) return false;
    table[I(symbol)]++;
    return true;
}

uint64_t * count_file(const char * file_name)
{
    std::ifstream file(file_name);
    if (!file.is_open()) return nullptr;
    char current;
    uint64_t * res = new_table();
    while (file.get(current)) res[I(current)]++;  
    file.close();
    return res;
}

int main()
{
    return 0;
}