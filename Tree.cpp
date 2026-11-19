#include "Tree.hpp"

#include <iostream>
#include <cstdint>
#include "Heap.hpp"
#include "Node.hpp"
#include "Code.hpp"

#define SIZE 512
#define SIZE_TREE 512
using index = std::uint16_t;
using big = std::uint32_t;
using byte = std::uint8_t;

HuffNode * get_tree(HuffNode ** heap, big * table)
{
    int size = get_size(heap);
    index extra = SIZE/2;

    while (size > 1)
    {
        HuffNode * Min = del(heap, size-1, table);
        HuffNode * Premin = del(heap, size-2, table);
        HuffNode * parent = get_parent(Min, Premin, table, extra);
        extra++;
        add(heap, parent, size-2, table);
        size--;
    }

    return heap[0];
}

void print_tree(HuffNode * root, bool with_code, int align)
{
    if (root == nullptr)
    {
        std::cout << "No tree\n";
        return;
    } 
    for (int i = 0; i < align; i++) std::cout << "|";
    std::cout << "~" <<root->pos << ' ';
    if (with_code) print_code(root->code);
    std::cout << '\n';
    if (root->left != nullptr) print_tree(root->left, with_code, align+1);
    if (root->right != nullptr) print_tree(root->right, with_code, align + 1);
}

HuffNode * erase_tree(HuffNode * root)
{
    if (root->left != nullptr) erase_tree(root->left);
    if (root->right != nullptr) erase_tree(root->right);
    return erase_node(root);
}
