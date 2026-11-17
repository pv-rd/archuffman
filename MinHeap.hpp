#pragma once

#include <iostream>
#include <cstdint>
#include "HuffmanTree.hpp"

int hrise(HuffNode ** array, int target);

int hdown(HuffNode ** array, int target, int size);

int hadd(HuffNode ** array, HuffNode * target, int end);

HuffNode * hdel(HuffNode ** array, int end);