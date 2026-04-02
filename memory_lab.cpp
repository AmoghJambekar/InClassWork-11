// Memory Management Lab — Part A (Rule of Three) + Part B (unique_ptr)
// Compile: c++ -std=c++17 -Wall -Wextra -fsanitize=address -g -o memory_lab memory_lab.cpp

#include <iostream>
#include <memory>
#include <string>
#include <utility>

using std::cout;
using std::endl;
using std::make_unique;
using std::string;
using std::unique_ptr;

// ---------------------------------------------------------------------------
// Part A — manual memory (raw new/delete)
// ---------------------------------------------------------------------------

struct ListNode {
    int id;
    string name;
    ListNode* next;
};

class NodeList {
private:
    ListNode* head;
    int count;

    static void freeList(ListNode* h) {
        while (h) {
            ListNode* n = h->next;
            delete h;
            h = n;
        }
    }

    static ListNode* copyChain(const ListNode* h) {
        if (!h) return nullptr;
        ListNode* newHead = new ListNode{h->id, h->name, nullptr};
        ListNode* tail = newHead;
        for (const ListNode* p = h->next; p; p = p->next) {
            tail->next = new ListNode{p->id, p->name, nullptr};
            tail = tail->next;
        }
        return newHead;
    }

public:
    NodeList() : head(nullptr), count(0) {}

    NodeList(const NodeList& other) : head(copyChain(other.head)), count(other.count) {}

    NodeList& operator=(const NodeList& other) {
        if (this == &other) return *this;
        ListNode* newHead = copyChain(other.head);
        freeList(head);
        head = newHead;
        count = other.count;
        return *this;
    }

    ~NodeList() { freeList(head); }

    void addNode(int id, string name) {
        ListNode* n = new ListNode{id, std::move(name), head};
        head = n;
        count++;
    }

    void removeNode(int id) {
        ListNode* prev = nullptr;
        ListNode* curr = head;
        while (curr && curr->id != id) {
            prev = curr;
            curr = curr->next;
        }
        if (!curr) return;
        if (prev)
            prev->next = curr->next;
        else
            head = curr->next;
        delete curr;
        count--;
    }

    void display() const {
        ListNode* curr = head;
        while (curr) {
            cout << "[" << curr->id << ": " << curr->name << "] -> ";
            curr = curr->next;
        }
        cout << "null" << endl;
    }

    int size() const { return count; }
};

// ---------------------------------------------------------------------------
// Part B — unique_ptr list (no raw new/delete in list operations)
// ---------------------------------------------------------------------------

struct SmartListNode {
    int id;
    string name;
    unique_ptr<SmartListNode> next;

    SmartListNode(int i, string n, unique_ptr<SmartListNode> nx)
        : id(i), name(std::move(n)), next(std::move(nx)) {}
};

class SmartNodeList {
private:
    unique_ptr<SmartListNode> head;
    int count;

public:
    SmartNodeList() : head(nullptr), count(0) {}

    void addNode(int id, string name) {
        head = make_unique<SmartListNode>(id, std::move(name), std::move(head));
        count++;
    }

    void removeNode(int id) {
        if (!head) return;
        if (head->id == id) {
            head = std::move(head->next);
            count--;
            return;
        }
        SmartListNode* curr = head.get();
        while (curr->next && curr->next->id != id) curr = curr->next.get();
        if (curr->next && curr->next->id == id) {
            curr->next = std::move(curr->next->next);
            count--;
        }
    }

    void display() const {
        SmartListNode* curr = head.get();
        while (curr) {
            cout << "[" << curr->id << ": " << curr->name << "] -> ";
            curr = curr->next.get();
        }
        cout << "null" << endl;
    }

    int size() const { return count; }
};

// ---------------------------------------------------------------------------
// Small demo (optional — remove or replace for Part C experiments)
// ---------------------------------------------------------------------------

int main() {
    cout << "=== Part A (NodeList) ===" << endl;
    NodeList a;
    a.addNode(1, "router");
    a.addNode(2, "switch");
    a.display();
    NodeList b = a;
    b.removeNode(1);
    cout << "copy after remove id 1: ";
    b.display();
    a = b;
    cout << "after assign: ";
    a.display();

    cout << "\n=== Part B (SmartNodeList) ===" << endl;
    SmartNodeList s;
    s.addNode(10, "a");
    s.addNode(20, "b");
    s.display();
    s.removeNode(10);
    s.display();

    return 0;
}
