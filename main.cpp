#include <iostream>
#include <fstream>
#include <cstdint>
#include <cstddef>

#define TABLE_SIZE 256

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
    return new uint64_t[TABLE_SIZE]();
}

void print_table(uint64_t * table)
{
    if (table == nullptr) std::cout << "No table\n";
    else
    {
        std::cout << "ASCII | Quantity\n";
        for (int i = 0; i < TABLE_SIZE; i++)
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

struct Node
{
    uint16_t index;
    Node * next;
    Node * L;
    Node * R;
};

Node * make_List(uint64_t * table)
{
    Node * tail = nullptr;
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        if (table[i] != 0)
        {
            tail = new Node{i, tail, nullptr, nullptr};
        }
    }
    return new Node{0, tail, nullptr, nullptr};
}

struct Minimums
{
    Node * first;
    Node * second;
};

void outList(Node * root, Node * target)
{
    Node * current = root;
    while (current->next != target)
    {
        current = current->next;
    }
    current->next = target->next;
}

uint64_t find_value(uint16_t index, uint64_t * table1, uint64_t * table2)
{
    if (index < TABLE_SIZE) return table1[index];
    else return table2[index - TABLE_SIZE];
}

Minimums * find_minimums(Node * root, uint64_t * table1, uint64_t * table2)
{
    if (root == nullptr || root->next == nullptr || root->next->next == nullptr) return nullptr;

    Minimums * res = new Minimums {nullptr, nullptr};
    uint64_t First = UINT64_MAX; uint64_t Second = UINT64_MAX;

    uint16_t Index = 0;
    uint64_t Value = 0;

    Node * current = root->next;
    while (current != nullptr)
    {
        Index = current->index;
        Value = find_value(Index, table1, table2);

        if (Value < First)
        {
            Second = First;
            res->second = res->first;
            First = Value;
            res->first = current;
        }
        else if (Value < Second)
        {
            Second = Value;
            res->second = current;
        }
        current = current->next;
    }
    return res;
}


Node * make_Tree(Node * root, uint64_t * table1, uint64_t * table2)
{
    uint16_t index = 0;
    uint64_t sum = 0;
    Minimums * M = find_minimums(root, table1, table2);
    if (M == nullptr) return nullptr;
    while (M != nullptr)
    {
        sum = find_value(M->first->index, table1, table2) +
            find_value(M->second->index, table1, table2);
        table2[index] = sum;
        index++; sum = 0;
        outList(root, M->first);
        outList(root, M->second);
        Node * parent = new Node{TABLE_SIZE + index - 1, root->next, M->second, M->first};
        root->next = parent;
        delete M;
        M = find_minimums(root, table1, table2);
    }
    return root->next;
}

void print_tree(Node * root)
{
    if (root == nullptr) return;

    std::cout << root->index << "(";
    print_tree(root->L);
    std::cout << ")[";
    print_tree(root->R);
    std::cout << "]";
}

int main()
{
    uint64_t * ASCII = count_file("test2.txt");
    Node * Prehead = make_List(ASCII);
    uint64_t * extra = new_table();
    Node * Root = make_Tree(Prehead, ASCII, extra);
    print_tree(Root);
    delete[] ASCII;
    delete[] extra;
    return 0;
}