#include <iostream>
#include <fstream>
#include <cstdint>
#include <cstddef>

#define SIZE 256

// построение таблицы частот

uint8_t I(char symbol) {return (uint8_t)symbol;}

char deI(uint8_t index) {return (char)index;}

uint32_t * new_table() {return new uint32_t[SIZE*2]();}

void print(uint32_t * table, bool full = false)
{
    if (table == nullptr) std::cout << "No table\n";
    else
    {
        std::cout << "N\t:\tQuantity\n";
        for (int i = 0; i < SIZE + SIZE*full; i++)
        {
            std::cout <<  i << "\t:\t" << table[i] << '\n';
        }
    }
}

bool table2file(uint32_t * table, const char * file_name, bool full = false)
{
    if (table == nullptr) return false;

    std::ofstream file(file_name);
    if (!file.is_open()) return false;
    for (int i = 0; i < SIZE + SIZE*full; i++)
    {
        file << i << ' ' << table[i] << '\n';
    }
    file.close();
    return true;
}

uint32_t * get_table(const char * file_name)
{
    std::ifstream file(file_name);
    if (!file.is_open()) return nullptr;
    char current;
    uint32_t * res = new_table();
    while (file.get(current)) res[I(current)]++;  
    file.close();
    return res;
}

struct Table
{
    uint32_t * Q; // частоты
    uint16_t * L; // индексы левых детей
    uint16_t * R; // индексы правых детей
    uint8_t ** C; // коды
    bool * used;
};

Table * create_Table(uint32_t * table)
{
    return new Table{table, new uint16_t [SIZE*2](), new uint16_t [SIZE*2](), new uint8_t* [SIZE*2](), new bool[SIZE*2]()};
}

struct M
{
    uint16_t Min;
    uint16_t Premin;
};

M find_minimums(Table * table, uint16_t extra = SIZE)
{
    uint32_t Min = UINT32_MAX; uint32_t Premin = UINT32_MAX;
    M res = {0,0};
    for (int i = 0; i < extra; ++i)
    {
        if (!table->used[i] && table->Q[i] != 0)
        {
            if (table->Q[i] < Min) 
            {
                res.Premin = res.Min;
                res.Min = i;
                Premin = Min;
                Min = table->Q[i];
            }
            else if (table->Q[i] < Premin) 
            {
                res.Premin = i;
                Premin = table->Q[i];
            }
        }
    }
    if (res.Premin == UINT32_MAX) res.Premin = res.Min;
    table->used[res.Min] = true;
    table->used[res.Premin] = true;
    std::cout << res.Min << ' ' << res.Premin << '\n';
    return res;
}

uint16_t make_Tree(Table * table)
{
    M m = find_minimums(table);
    uint16_t extra = SIZE;
    while (m.Min != m.Premin)
    {
        table->Q[extra] = table->Q[m.Min] + table->Q[m.Premin];
        table->L[extra] = m.Min;
        table->R[extra] = m.Premin;
        extra ++;
        m = find_minimums(table, extra);
    }
    return extra-1;
}

void print(Table * table, uint16_t start, int align = 0)
{
    if (table == nullptr) return;

    for (int i = 0; i < align; i++) std::cout << "|";
    std::cout << "~" << start << '\n';
    //if (with_code) {std::cout << " : "; print_code(root->code);}
    //std::cout << std::endl;
    if (table->L[start] != table->R[start]) 
    {
        print(table, table->L[start], align+1);
        print(table, table->R[start], align+1);
    }
}


int main()
{
    const char * name = "test.txt"; 
    uint32_t * table = get_table(name);
    print(table);
    Table * T = create_Table(table);
    uint16_t H = make_Tree(T);
    print(T, H);
    return 0;
}