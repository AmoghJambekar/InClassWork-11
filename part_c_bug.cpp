// Same as part_c_leak.cpp — empty destructor leak (Part C bug 1).
// Build: c++ -std=c++17 -g -fsanitize=address -fno-omit-frame-pointer -o buggy_part_c part_c_bug.cpp

#include <iostream>
#include <string>

struct ListNode {
    int id;
    std::string name;
    ListNode* next;
};

class NodeList {
    ListNode* head;

public:
    NodeList() : head(nullptr) {}

    void addNode(int id, std::string name) {
        head = new ListNode{id, std::move(name), head};
    }

    ~NodeList() {}  // BUG: should delete chain
};

int main() {
    NodeList list;
    list.addNode(1, "a");
    list.addNode(2, "b");
    return 0;
}
