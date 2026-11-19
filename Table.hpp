#pragma once

#include <iostream>
#include <fstream>
#include <cstdint>

std::uint32_t * new_table(); // создаёт таблицу
std::uint32_t * get_table(const char * file_name, bool count_size = true); // возвращает таблицу частот
std::uint32_t get_size(const char * file_name); // возвращает размер файла в байтах
int count_nonzero(std::uint32_t * table); // считает количество ненулевых ячеек
enum PrintMode : char {Full, Half, Nonzero};
void print_table(std::uint32_t * table, PrintMode mode = PrintMode::Half);
void erase_table(std::uint32_t * table);