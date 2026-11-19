#include "Coder.hpp"

#include <iostream>
#include <cstdint>
#include "Heap.hpp"
#include "Node.hpp"
#include "Code.hpp"
#include "Tree.hpp"

#define SIZE 512
#define SIZE_TREE 512
using index = std::uint16_t;
using big = std::uint32_t;
using byte = std::uint8_t;

void set_code_tree(HuffNode * root, byte ** tree, int i)
{
    if (i < 0 || i >= SIZE_TREE) return;
    if (root == nullptr) {tree[i] = new byte[2](); return;}
    int size = byte_size(root->code[0])+2;
    tree[i] = new byte[size]();
    for (int j = 0; j < size-1; j++)
        tree[i][j] = root->code[j];
    tree[i][size-1] = root->pos; 
    if (root->left == nullptr && root->right == nullptr) return;
    set_code_tree(root->left, tree, 2*i+1);
    set_code_tree(root->right, tree, 2*i+2);
}

byte ** new_code_tree()
{
    return new byte*[SIZE_TREE]();
}

void print_code_tree(byte ** tree)
{
    for (int i = 0; i < SIZE_TREE; i++)
    {
        std::cout << i << " : ";
        if (tree[i] == nullptr) std::cout << "NULL\n";
        else {print_code(tree[i]); std::cout << ' ' << (int) tree[i][byte_size(tree[i][0])+1] <<'\n';}
    }
}