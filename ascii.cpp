#include <iostream>
#include <fstream>
#include <cstdint>
#include <cstddef>

#define SIZE 256

// построение таблицы частот

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
    return new uint64_t[SIZE]();
}

void print_table(uint64_t * table, bool only_ascii = true)
{
    if (table == nullptr) std::cout << "No table\n";
    else
    {
        std::cout << "N\t:\tQuantity\n";
        for (int i = 0; i < ((only_ascii) ? SIZE/2 : SIZE); i++)
        {
            std::cout <<  i << "\t:\t" << table[i] << '\n';
        }
    }
}

bool table2file(uint64_t * table, const char * file_name, bool only_ascii = true)
{
    if (table == nullptr) return false;
    std::ofstream file(file_name);
    if (!file.is_open()) return false;
    for (int i = 0; i < ((only_ascii) ? SIZE/2 : SIZE); i++)
    {
        file << (char)i << ' ' << table[i] << '\n';
    }
    file.close();
    return true;
}

bool table_is_correct(uint64_t * table)
{
    if (table==nullptr) return false;
    for (int i = SIZE/2; i < SIZE; i++){
        if (table[i]) return false;
    }
    return true;
}

uint64_t * file2table(const char * file_name)
{
    std::ifstream file(file_name);
    if (!file.is_open()) return nullptr;
    char current;
    uint64_t * res = new_table();
    while (file.get(current)) if(I(current) < SIZE/2) res[I(current)]++;  
    file.close();
    return res;
}

// Связный список с фиктивной головой

struct Node
{
    uint8_t index;
    Node * next;
    Node * L;
    Node * R;
    uint8_t * code;
};

Node * new_Node(uint8_t index, 
    Node * next = nullptr, 
    Node * left = nullptr,
    Node * right = nullptr, 
    uint8_t * code = nullptr)
    {
        return new Node {index, next, left, right, code};
    }

Node * create_List(uint8_t head_value = 0)
{
    return new_Node(head_value);
}

Node * add_to_List(Node * head, uint8_t index)
{
    if (head == nullptr) return create_List(index);
    Node * res = new_Node(index, head->next);
    head->next = res;
    return res;
}

Node * go_out_List(Node * head, Node * target)
{
    if (head == nullptr || target == nullptr) return nullptr;
    Node * current = head;
    while (current ->next != nullptr && current->next != target)
        current = current->next;
    if (current->next == nullptr) return nullptr;
    current->next = target->next;
    return current;
}

Node * table2list(uint64_t * table)
{
    if (!table_is_correct(table)) return nullptr;
    Node * res = create_List();
    for (int i = 0; i < SIZE/2; i++)
        if (table[i]) add_to_List(res, (uint8_t)i);
    return res;
}

void print_list(Node * root, uint64_t * table)
{
    if (root == nullptr) std::cout << "No list";
    Node * current = root;
    while (current != nullptr) 
    {
        std::cout << (int)current->index << ":" << table[current->index] <<" -> ";
        current = current->next;
    }
    std::cout <<"End\n";
}

// построение дерева Хаффмана

struct Min
{
    Node * first;
    Node * second;
};

Min * find_mins(Node * root, uint64_t * table)
{
    if (root == nullptr || root->next == nullptr) return nullptr;

    Min * res = new Min {nullptr, nullptr};

    if (root->next->next == nullptr) 
    {
        res->first = root->next; return res;
    }

    Node * current = root->next;
    uint64_t First = UINT64_MAX; uint64_t Second = UINT64_MAX;

    while (current != nullptr)
    {
        
        uint64_t Value = table[current->index];

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

Node * make_Tree(Node * root, uint64_t * table)
{
    if (root == nullptr || table == nullptr) return nullptr;
    
    Min * M = find_mins(root, table);
    if (M == nullptr) return nullptr;
    uint16_t extra = 128;
    while (M->second != nullptr)
    {
        table[extra] = table[M->first->index] + table[M->second->index];
        if(!go_out_List(root, M->first) || !go_out_List(root, M->second)) return nullptr;
        Node * parent = new_Node(extra, root->next, M->first, M->second);
        extra++;
        root->next = parent;
        delete M;
        M = find_mins(root, table);
    }
    if (root->next->index < 128) root->next = new_Node(extra, root->next, M->first);
    delete M;
    return root->next;
}

void print_code(uint8_t * code)
{
    if (code!=nullptr)
    {
        std::cout << (int)code[0] << " : ";
        for (int i = 1; i <=(code[0] - 1) / 8 + 1; i++) 
        {
            for (int j = 0; j < 8; j++)
            {
                if ((i-1)*8 + j >= code[0]) break;
                std::cout << (bool)(code[i]&(0b10000000 >> j));
            }
        }
    }
}


void print_Tree(Node * root, bool with_code = false, int align = 0)
{
    if (root == nullptr) return;

    for (int i = 0; i < align; i++) std::cout << "|";
    std::cout << "~" <<(char)root->index;
    if (with_code) {std::cout << " : "; print_code(root->code);}
    std::cout << std::endl;
    print_Tree(root->L, with_code, align+1);
    print_Tree(root->R, with_code, align + 1);
}

// кодируем массивом байтов, первый байт - длина кода в битах

void code_Tree(Node * root, uint8_t * code = nullptr)
{
    if (root == nullptr) return;
    if (code == nullptr)
    {
        uint8_t * left = new uint8_t[2]();
        uint8_t * right = new uint8_t[2]();
        left[0] = 1; right[0] = 1;
        right[1] = 0b10000000;
        code_Tree(root->L, left);
        code_Tree(root->R, right);
        return;
    }
    if (root->L == nullptr && root->R == nullptr)
    {
        root->code = code;
        return;
    }
    root->code = code;
    uint8_t bits = code[0];
    uint8_t bytes = (bits - 1) / 8 + 1;
    uint8_t address = (bits - 1) % 8;
    uint8_t new_bits = bits + 1;
    uint8_t new_bytes = bytes;
    if (address == 7) new_bytes++;
    uint8_t new_address = (address + 1) % 8;
    uint8_t * left = new uint8_t[new_bytes + 1]();
    uint8_t * right = new uint8_t[new_bytes + 1]();
    left[0] = new_bits; right[0] = new_bits;
    for (int i = 1; i <= bytes; ++i) 
    {
        left[i] = code[i]; right[i] = code[i];
    }
    right[new_bytes] = right[new_bytes] | (0b10000000 >> new_address);
    code_Tree(root->L, left);
    code_Tree(root->R, right);
}

uint8_t ** code_Table(Node * root, uint8_t ** code_table = nullptr)
{
    if (root == nullptr) return nullptr;
    if (code_table == nullptr)
    {
        code_table = new uint8_t * [SIZE/2] ();
    }
    if (root->index < 128)
    {
        code_table[root->index] = root->code; return code_table;
    }
    code_Table(root->L, code_table); code_Table(root->R, code_table);
    return code_table;
}

void print_code_table( uint8_t ** code_table)
{
    for (int i = 0; i < SIZE/2; i++){
        std::cout << i << " : ";
        print_code(code_table[i]);
        std::cout << '\n';
    }
}

bool code_file(uint8_t ** code_table, const char * input_name, const char * output_name = "compressed.txt")
{
    std::ifstream input(input_name);
    std::ofstream output(output_name);
    if (!input.is_open() || !output.is_open()) return false;
    char in;
    char out;
    uint8_t buffer = 0;
    uint8_t pos = 0;
    while (input.get(in)) {
        if (I(in) < SIZE/2) 
        {
            uint8_t * code = code_table[I(in)];
            uint8_t len = code[0];
            for (int i = 0; i < len; i++)
            {
                buffer = buffer | ((0b10000000 >> pos) & (code[i / 8 + 1] & (0b10000000 >> i%8)));
                pos++;
                if (pos > 7)
                {
                    output << (char)buffer;
                    buffer = 0;
                    pos = 0;
                }
            }
        }
    }
    if (pos != 0) output << (char) buffer;
    input.close(); output.close();
    return true;
}

int main()
{
    uint64_t * Table = file2table("image.jpeg");
    Node * Head = table2list(Table);
    print_list(Head, Table);
    Node * Root = make_Tree(Head, Table);
    code_Tree(Root);
    print_Tree(Root, true);
    print_code_table(code_Table(Root));
    code_file(code_Table(Root), "image.jpeg");
    return 0;
}