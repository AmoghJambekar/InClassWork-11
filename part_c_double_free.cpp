// Part C — Bug 2: double free
// Build: c++ -std=c++17 -g -fsanitize=address -o part_c_double_free part_c_double_free.cpp
// Run:   ./part_c_double_free
// Expect: AddressSanitizer reports double-free or invalid free.

#include <iostream>
#include <string>

struct ListNode {
    int id;
    std::string name;
    ListNode* next;
};

int main() {
    ListNode* n = new ListNode{1, "solo", nullptr};
    delete n;
    delete n;  // BUG: second delete on same pointer
    return 0;
}
