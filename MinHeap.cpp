#include "MinHeap.hpp"
#include "HuffmanTree.hpp"

#include <iostream>
#include <cstdint>

#define SIZE 512

using index = std::uint16_t;
using big = std::uint32_t;
using byte = std::uint8_t;

int hrise(HuffNode ** array, int target)
{
    if (target == 0) return 0;
    int parent = (target - 1) / 2;
    if (*(array[parent]->ptr) < *(array[target]->ptr)) return target;
    std::swap(array[parent], array[target]);
    return hrise(array, parent);
}

int hdown(HuffNode ** array, int target, int size)
{
    if (target == size) return target;
    int L = 2*target + 1;
    int R = 2*target + 2;
    int child = L;
    if (*(array[R]->ptr) < *(array[L]->ptr)) child = R;
    if (*(array[child]->ptr) < *(array[target]->ptr))
    {
        std::swap(array[child], array[target]);
        return hdown(array, child, size);
    }
    return target;
}

int hadd(HuffNode ** array, HuffNode * target, int end)
{
    array[end] = target;
    return hrise(array, end);
}

HuffNode * hdel(HuffNode ** array, int end)
{
    HuffNode * res = array[0];
    std::swap(array[end], array[0]);
    hdown(array, 0, end);
    return res;
}

