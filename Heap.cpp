#include "Heap.hpp"

#include <iostream>
#include <cstdint>
#include "Node.hpp"

#define SIZE 512
using index = std::uint16_t;
using big = std::uint32_t;
using byte = std::uint8_t;


// минимальная куча. Храним в массиве

int rise(HuffNode ** heap, int i, big * table) // просеивание вверх элемента на индексе i
{
    if (heap == nullptr || table == nullptr || i >= SIZE) return -1;
    if (i <= 0) return 0;
    int parent = (i - 1) / 2;
    int res = i;
    if (get_value(heap[i], table) < get_value(heap[parent], table)) 
    {
        std::swap(heap[i], heap[parent]);
        res = rise(heap, parent, table);
    }
    return res;
}

int down(HuffNode ** heap, int i, big * table)
{
    if (heap == nullptr || table == nullptr || i < 0 || i >= SIZE) return -1;

    int left = 2*i + 1;
    int right = 2*i + 2;
    int child = -1;

    if (left < SIZE && heap[left] != nullptr) child = left;
    if (right < SIZE && heap[right] != nullptr &&
        (child == -1 || get_value(heap[right], table) < get_value(heap[child], table)))
        child = right;

    if (child == -1) return i;
    if (get_value(heap[child], table) < get_value(heap[i], table))
    {
        std::swap(heap[i], heap[child]);
        return down(heap, child, table);
    }
    return i;
}

int add(HuffNode ** heap, HuffNode * node, int end, big * table)
{
    if (end >= SIZE || end < 0) return -1;
    if (heap[end] != nullptr) return -1;
    heap[end] = node;
    return rise(heap, end, table);
}

HuffNode * del(HuffNode ** heap, int end, big * table)
{
    if (end >= SIZE || end < 0) return nullptr;
    HuffNode * res = heap[0];
    heap[0] = heap[end];
    heap[end] = nullptr;
    if (end > 0) down(heap, 0, table);
    return res;
}

HuffNode ** get_heap(big * table)
{
    HuffNode ** heap = new HuffNode*[SIZE]();
    int end = 0;
    for (int i = 0; i < SIZE; i++)
    {
        if (table[i] > 0)
        {
            HuffNode * node = new_node(i);
            add(heap, node, end, table);
            end++;
        }
    }
    return heap;
}

int get_size(HuffNode ** heap)
{
    int start = SIZE - 1;
    while (start > 0 && heap[start] == nullptr) start--;
    return start+1;
}

void print_heap(HuffNode ** heap, big * table)
{
    for (int i = 0; i < get_size(heap); i++)
    {
        std::cout << i << " : ";
        if (heap[i] == nullptr) std::cout << "NULL\n";
        else
        {
            std::cout << heap[i]->pos << " " << get_value(heap[i], table) << '\n';
        }
    }
}
