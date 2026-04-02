// Part C — Bug 1: leak (destructor does not free nodes)
// Build: c++ -std=c++17 -g -fsanitize=address -o part_c_leak part_c_leak.cpp
// Run:   ./part_c_leak
// Linux + ASan: LeakSanitizer often reports leaks at exit.
// macOS: Clang ASan may NOT report leaks (detect_leaks unsupported). Use
//   Xcode → Open Developer Tool → Instruments → Leaks → choose this binary,
//   or run the same code on Linux with Valgrind / ASan for a text report.

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

    // BUG: empty body — nodes never deleted (memory leak)
    ~NodeList() {}
};

int main() {
    NodeList list;
    list.addNode(1, "a");
    list.addNode(2, "b");
    return 0;
}
