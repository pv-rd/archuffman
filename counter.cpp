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

void print_table(uint64_t * table)
{
    if (table == nullptr) std::cout << "No table\n";
    else
    {
        std::cout << "ASCII code | Quantity\n";
        for (int i = 0; i < 0b100000000; i++)
            std::cout <<  i << "\t|\t" << table[i] << '\n';
    }
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

uint64_t * ptr(uint8_t ascii, uint64_t * table)
{
    return table + ascii;
}

uint8_t mask(uint8_t target)
{
    uint8_t res = 0b10000000;
    return res >> target;
}


bool coder(const char * input_name, const char * output_name, char ** table)
{
    std::ifstream input(input_name);
    std::ofstream output(output_name, std::ios::binary);
    if (!input.is_open() || !output.is_open()) return false;

    char current;
    char buffer = 0;
    char * code;
    uint8_t bufferi = 0;
    uint8_t codei = 0;
    while (input.get(current))
    {
        codei = 0;
        code = table[I(current)];
        while (bufferi < 8 && code[codei] != '\0')
        {
            buffer = buffer | (mask(bufferi) & code[codei]);
            bufferi++;
            codei++;
        }
        if (bufferi == 8)
        {
            output.put(buffer);
            buffer = 0;
            bufferi = 0;
            while (code[codei] != '\0')
            {
                buffer = buffer | (mask(bufferi) & code[codei]);
                bufferi++;
                codei++;
            }
        }
    }  
    if (bufferi > 0) output.put(buffer);
    input.close();
    output.close();
    return true;
}

struct Leaf {
    uint8_t c;
    uint64_t * q;
    Leaf * l;
    Leaf * r;
    Leaf * next;
};

Leaf * make_List(uint64_t * table)
{
    Leaf * Head = nullptr;
    for (int i = 0; i < 256; ++i)
    {
        if (table[i] > 0) Head = new Leaf{i, table + i, nullptr, nullptr, Head};
    }
    Head = new Leaf{0, 0, nullptr, nullptr, Head};
    return Head;
}

bool del(Leaf * target, Leaf * Head)
{
    Leaf * current = Head;
    while (current->next != target && current -> next != nullptr) current = current->next;
    if (current->next == target) 
    {
        if (target == nullptr) return false;
        current->next = target->next;
        return true;
    }
    return false;
}

bool push(Leaf * target, Leaf * Head)
{
    Leaf * current = Head;
    while (current -> next != nullptr) current = current->next;
    current->next = target;
    return true;

}

struct M {Leaf * m; Leaf * prem;};

M * find_mins(Leaf * Head)
{
    M * res = new M{Head, Head};
    uint64_t Min = UINT64_MAX;
    uint64_t Premin = UINT64_MAX;
    Leaf * current = Head->next;
    while(current != nullptr)
    {
        uint64_t val = *(current->q);
        if (val < Min)
        {
            Premin = Min;
            res->prem = res->m;
            Min = val;
            res->m = current;
        }
        else if (val < Premin)
        {
            Premin = val;
            res->prem = current;
        }
        current = current->next;
    }
    return res;
}

Leaf * list2tree(Leaf * Head)
{
    uint64_t * add = new uint64_t[256];
    uint8_t index = 0;
    while (Head->next->next != nullptr)
    {
        M * mins = find_mins(Head);

        uint64_t Qm = *(mins->m->q);
        uint64_t Qprem = *(mins->prem->q);
        add[index] = Qm + Qprem;
        Leaf * NL = new Leaf{I('X'), add + index, mins->prem, mins->m, nullptr};
        index++;
        del(mins->m, Head);
        del(mins->prem, Head);
        delete mins;
        push(NL, Head);
    }
    return Head->next;
}

void print_tree(Leaf * Head)
{
    if (Head == nullptr) return;
    std::cout << (char)Head->c << ":L(";
    print_tree(Head -> l);
    std::cout << ") R[";
    print_tree(Head -> r);
    std::cout << "]";
}

int main()
{
    print_tree(list2tree(make_List(count_file("test.txt"))));
    return 0;
}