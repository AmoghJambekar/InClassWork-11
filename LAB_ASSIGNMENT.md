# Lab Assignment

This lab focuses on hands-on memory management: you will build the same data structure twice (manual and smart pointers), detect and fix leaks using tools, and compare the approaches.

## Lab: Memory Management Tools and Techniques

### Part A - Manual Memory Management

Build a linked list of network nodes using raw `new` and `delete`. Implement:

- `addNode(int id, string name)`
- `removeNode(int id)`
- `display()`
- Proper destructor, copy constructor, and assignment operator (Rule of Three)

#### Part A starter code

```cpp
struct ListNode {
    int id;
    string name;
    ListNode* next;
};

class NodeList {
private:
    ListNode* head;
    int count;

public:
    NodeList() : head(nullptr), count(0) {}

    void addNode(int id, string name) {
        ListNode* n = new ListNode{id, name, head};
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
        if (prev) prev->next = curr->next;
        else head = curr->next;
        delete curr;
        count--;
    }

    void display() const {
        ListNode* curr = head;
        while (curr) {
            cout << "[" << curr->id << ": "
                 << curr->name << "] -> ";
            curr = curr->next;
        }
        cout << "null" << endl;
    }

    // TODO: Implement destructor, copy constructor, operator=
    // (Part A deliverable)

    ~NodeList() {
        // Your code: free all nodes
    }
};
```

### Part B - Smart Pointer Rewrite

Rewrite the same `NodeList` using `unique_ptr`. Notice how much simpler it becomes. Compare:

- Lines of code for memory management
- Risk of memory bugs
- Whether you need Rule of Three at all

#### Part B - Smart pointer version

```cpp
struct SmartListNode {
    int id;
    string name;
    unique_ptr<SmartListNode> next;

    SmartListNode(int i, string n, unique_ptr<SmartListNode> nx)
        : id(i), name(n), next(move(nx)) {}
};

class SmartNodeList {
private:
    unique_ptr<SmartListNode> head;
    int count;

public:
    SmartNodeList() : head(nullptr), count(0) {}

    void addNode(int id, string name) {
        head = make_unique<SmartListNode>(id, name, move(head));
        count++;
    }

    void display() const {
        SmartListNode* curr = head.get();
        while (curr) {
            cout << "[" << curr->id << ": "
                 << curr->name << "] -> ";
            curr = curr->next.get();
        }
        cout << "null" << endl;
    }

    // No destructor needed! No copy constructor needed!
    // unique_ptr handles all cleanup automatically.
};
```

### Part C - Leak Detection

Intentionally introduce these bugs into your Part A code and verify that Valgrind catches each one:

1. Comment out the destructor body (memory leak)
2. Delete a node twice (double free)
3. Access a node after deleting it (use-after-free)

Record the Valgrind output for each bug in your lab report.

### Submission checklist

- Part A: Manual `NodeList` with full Rule of Three implementation
- Part B: Smart pointer `SmartNodeList` (no raw new/delete)
- Part C: Screenshots of Valgrind detecting 3 different bugs
- Written comparison: lines of code, safety, and complexity differences
- Clean compilation with `-Wall -Wextra -fsanitize=address`
- GitHub repo with at least 3 meaningful commits
