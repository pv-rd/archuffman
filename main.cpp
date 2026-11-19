#include <iostream>
#include <fstream>
#include <cstdint>
#include "Table.hpp"
#include "Code.hpp"
#include "Node.hpp"
#include "Heap.hpp"
#include "Tree.hpp"
#include "Coder.hpp"

constexpr int SIZE = 256;

using byte = std::uint8_t;
using index = std::uint16_t;
using big = std::uint32_t;


int main()
{
    const char * FILE = "test.txt";
    big * TABLE = get_table(FILE);
    HuffNode ** HEAP = get_heap(TABLE);
    print_heap(HEAP, TABLE);
    HuffNode * TREE = get_tree(HEAP, TABLE);
    set_code(TREE);
    print_tree(TREE, true);
    byte ** CODE_TREE = new_code_tree();
    set_code_tree(TREE, CODE_TREE);
    print_code_tree(CODE_TREE);
    return 0;
}