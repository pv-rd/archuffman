#include <iostream>
#include <fstream>
#include <cstdint>

constexpr int SIZE = 256;

using byte = std::uint8_t;
using index = std::uint16_t;
using big = std::uint32_t;

big * new_table()
{
    return new big[SIZE*2]();
}

big * get_table(const char * file_name)
{
    std::ifstream file(file_name, std::ios::binary);
    if (!file.is_open()) return nullptr;
    char current;
    big * res = new_table();
    while (file.get(current)) res[(index)(unsigned char)current]++;  
    file.close();
    return res;
}

int count_table(big * table)
{
    int res = 0;
    for (int i = 0; i < SIZE*2; i++) if (table[i] > 0) res++;
    return res;
}

// Связный список с фиктивной головой

struct Node
{
    index pos;
    Node * next;
    Node * left;
    Node * right;
    byte * code;
};

Node * new_Node(index index, Node * next = nullptr, Node * left = nullptr, Node * right = nullptr, byte * code = nullptr)
{
    return new Node {index, next, left, right, code};
}

Node * new_list(index head = 0)
{
    return new_Node(head);
}

Node * add(Node * head, index i)
{
    if (head == nullptr) return nullptr;
    if (head == nullptr) return new_list(i);
    Node * res = new_Node(i, head->next);
    head->next = res;
    return res;
}

Node * out(Node * head, Node * target)
{
    if (head == nullptr || target == nullptr) return nullptr;
    Node * current = head;
    while (current->next != nullptr && current->next != target) current = current->next;
    if (current->next == nullptr) return nullptr;
    current->next = target->next;
    return current;
}

Node * get_list(big * table)
{
    if (table == nullptr) return nullptr;
    Node * res = new_list();
    for (int i = 0; i < SIZE; i++)
        if (table[i]) if(!add(res, (index)i)) return nullptr;
    return res;
}

// построение дерева Хаффмана

struct Min
{
    Node * first;
    Node * second;
};

Min * find_mins(Node * root, big * table)
{
    if (root == nullptr || root->next == nullptr) return nullptr;

    Min * res = new Min {nullptr, nullptr};
    Node * current = root->next;
    big First = UINT32_MAX; big Second = UINT32_MAX;

    while (current != nullptr)
    {
        big Value = table[current->pos];
        if (Value < First)
        {
            Second = First;
            res->second = res->first;
            First = Value;
            res->first = current;
        }
        else if (Value <= Second)
        {
            Second = Value;
            res->second = current;
        }
        current = current->next;
    }
    return res;
}

Node * get_tree(Node * root, big * table)
{
    if (root == nullptr || table == nullptr) return nullptr;

    Min * M = find_mins(root, table);
    if (M == nullptr) return nullptr;
    index extra = SIZE;
    while (M->second != nullptr)
    {
        table[extra] = table[M->first->pos] + table[M->second->pos];
        if(!out(root, M->first) || !out(root, M->second))
        {
            delete M;return nullptr;
        } 
        Node * parent = new_Node(extra, root->next, M->first, M->second);
        extra++;
        root->next = parent;
        delete M;
        M = find_mins(root, table);
    }
    delete M;
    return root->next;
}


// кодируем массивом байтов, первый байт - длина кода в битах

bool get_bit(byte Byte, int pos = 0)
{
    byte mask = 0b10000000 >> pos;
    return Byte & mask;
}

byte set_bit(byte Byte, int pos, bool bit = true)
{
    byte mask = 0b10000000 >> pos;
    if (bit)
        return Byte | mask;
    else
        return Byte & (~mask);
}

int byte_size(int bit_size)
{
    if (bit_size == 0) return 0;
    return (bit_size - 1) / 8 + 1;
}

bool set_code(Node * root, byte * code = nullptr)
{
    if (root == nullptr) return false;
    if (root->left == nullptr && root->right == nullptr && code == nullptr)
    {
        root->code = new byte[2]();
        root->code[0] = 1;
        return true;
    }
    if (code == nullptr)
    {
        code = new byte[1]();
    }
    if (root->left == nullptr && root->right == nullptr)
    {
        root->code = code;
        return true;
    }

    root->code = code;
    int pos = code[0];
    int length = byte_size(pos);
    if (pos % 8 == 0) length++;
    
    byte * left = new byte[length+1]();
    byte * right = new byte[length+1]();

    left[0] = pos+1; right[0] = pos+1;

    for (int i = 1; i < byte_size(pos) + 1; ++i)
    {
        left[i] = code[i]; right[i] = code[i];
    }

    right[length] = set_bit(right[length], pos % 8);

    bool res1 = set_code(root->left, left);
    bool res2 = set_code(root->right, right);
    return res1 && res2;
}

bool write_code_table(Node * root, byte ** code_table)
{
    if (root == nullptr) return false;
    if (root->left == nullptr && root->right == nullptr) 
    {
        code_table[root->pos] = root->code;
        return true;
    }
    bool res1 = write_code_table(root->left, code_table);
    bool res2 = write_code_table(root->right, code_table);
    return res1 && res2;
}

bool code_file(byte ** code_table, const char * input_name, const char * output_name)
{
    std::ifstream input(input_name, std::ios::binary);
    std::ofstream output(output_name, std::ios::binary);
    if (!input.is_open() || !output.is_open()) return false;
    
    byte N = 255;
    for (int i = 0; i < SIZE; i++) if (code_table[i] !=nullptr) N++;
    output << (char)N;

    for (int i = 0; i < SIZE; i++)
    {
        if (code_table[i] != nullptr)
        {
            output << (char)i;
            for (int j = 0; j < byte_size(code_table[i][0]) + 1; j++) 
                output << (char)code_table[i][j];
        }
    }

    char in;
    char out;
    byte buffer = 0;
    int pos = 0;
    while (input.get(in)) {
        byte * code = code_table[(index)(unsigned char)in];
        int bit_length = code[0];
        for (int i = 0; i < bit_length; i++)
        {
            buffer = set_bit(buffer, pos, get_bit(code[byte_size(i) + 1], i%8));
            pos++;
            if (pos > 7)
            {
                output << (char)buffer;
                buffer = 0;
                pos = 0;
            }
        }
    }
    if (pos > 0) output << (char) buffer;
    input.close(); output.close();
    return true;
}


bool linearize(Node * root, byte ** array, index i, big * table)
{
    if (root == nullptr) return true;
    if (array == nullptr) return false;
    if (array[i] != nullptr) return false;

    int length = byte_size(root->code[0]) + 2;
    byte * code = new byte[length];
    byte symbol = root->pos;
    code[0] = symbol;
    code[1] = root->code[0];
    for (int i = 2; i < length; i++) code[i] = root->code[i-1];
    array[i] = code;
    bool res1 = linearize(root->left, array, 2*i+1, table);
    bool res2 = linearize(root->right, array, 2*i+2, table);
    return res1 && res2;
}

// очистка дерева

void erase(Node * root)
{
    if (root == nullptr) return;
    if (root->left == nullptr && root->right == nullptr) 
    {
        delete[] root->code;
        delete root;
        return;
    }
    erase(root->left); erase(root->right);
    delete[] root->code;
    delete root;
}

// Вывод

void print(big * table, bool full = false)
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

void print(Node * root, big * table)
{
    if (root == nullptr) std::cout << "No list";
    Node * current = root;
    while (current != nullptr) 
    {
        std::cout << (int)current->pos << ":" << table[current->pos] <<" -> ";
        current = current->next;
    }
    std::cout <<"End\n";
}

void print(byte * code)
{
    if (code != nullptr)
    {
        for (int i = 1; i < byte_size(code[0]) + 1; i++) 
        {
            for (int j = 0; j < 8; j++)
            {
                if ((i-1)*8 + j >= code[0]) break;
                std::cout << get_bit(code[i], j);
            }
        }
    }
}

void print(Node * root, bool with_code = false, int align = 0)
{
    if (root == nullptr) return;
    for (int i = 0; i < align; i++) std::cout << "|";
    std::cout << "~" <<root->pos;
    if (with_code) {std::cout << " : "; print(root->code);}
    std::cout << std::endl;
    print(root->left, with_code, align+1);
    print(root->right, with_code, align + 1);
}

void print(byte ** code_table)
{
    for (int i = 0; i < SIZE; i++){
        std::cout << i << " : ";
        print(code_table[i]);
        std::cout << '\n';
    }
}

void printl(byte ** line)
{
    for (int i = 0; i < SIZE*2; i++){
        if (line[i] != nullptr) print(line[i]);
        std::cout << '\n';
    }
}

// всё вместе

int coder (const char * input_name, 
    const char * output_name = "compressed.hhh", 
    bool verbose = true)
{
    big * frequencies = get_table(input_name);
    if (frequencies == nullptr)
    {
        if (verbose) std::cout << "Error: file is not opened\n";
        return 1; 
    }
    else if (verbose) print(frequencies);

    Node * head = get_list(frequencies);
    if (head == nullptr)
    {
        if (verbose) std::cout << "Error: list is not made\n";
        return 2;
    }else if (verbose) print(head, frequencies);

    Node * tree = get_tree(head, frequencies);
    if (tree == nullptr)
    {
        if (verbose) std::cout << "Error: tree is not made\n";
        return 3;
    }else if (verbose) print(tree);

    if (!set_code(tree))
    {
        if (verbose) std::cout << "Error: codes is not set\n";
        return 4;
    }

    byte ** code_table = new byte*[SIZE]();
    if (!write_code_table(tree, code_table)) 
    {
        if (verbose) std::cout << "Error: code table is not made\n";
        return 5;
    }else if (verbose) print(code_table);

    if (!code_file(code_table, input_name, output_name)) 
    {
        if (verbose) std::cout << "Error: input or output file is not opened or made\n";
        return 6;
    }

    byte ** Line = new byte*[SIZE*3]();
    linearize(tree, Line, 0, frequencies);
    printl(Line);
    
    delete[] code_table;
    erase(tree);
    delete[] frequencies;
    return 0;
}

int main()
{
    coder("test3.txt");
    return 0;
}