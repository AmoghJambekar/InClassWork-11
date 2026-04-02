// Part C — Bug 3: use-after-free
// Build: c++ -std=c++17 -g -fsanitize=address -o part_c_uaf part_c_uaf.cpp
// Run:   ./part_c_uaf
// Expect: AddressSanitizer reports heap-use-after-free.

#include <iostream>
#include <string>

struct ListNode {
    int id;
    std::string name;
    ListNode* next;
};

int main() {
    ListNode* n = new ListNode{42, "x", nullptr};
    delete n;
    // BUG: read through freed pointer
    std::cout << n->id << std::endl;
    return 0;
}
