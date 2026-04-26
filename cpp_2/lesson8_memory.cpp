#include <iostream>
#include <vector>

int main() {

    // ── PART 1: Stack memory ─────────────────────────────
    std::cout << "=== STACK ===\n";

    // lives on the stack -- automatic cleanup
    int a = 42;
    double b = 3.14;
    std::cout << "a = " << a << "\n";
    std::cout << "b = " << b << "\n";
    // when main() ends, a and b are automatically cleaned up

    // ── PART 2: Heap memory ──────────────────────────────
    std::cout << "\n=== HEAP ===\n";

    // new --> allocate on heap
    // like Python's  x = SomeObject()
    int* p = new int(100);
    std::cout << "heap value    : " << *p       << "\n";
    std::cout << "heap address  : " << p        << "\n";

    // change the value
    *p = 200;
    std::cout << "after change  : " << *p << "\n";

    // delete --> free the memory
    // MUST do this or memory leak!
    delete p;
    std::cout << "memory freed!\n";

    // ── PART 3: Heap array ───────────────────────────────
    std::cout << "\n=== HEAP ARRAY ===\n";

    // allocate array of 5 doubles on heap
    // like numpy array but manual
    int size = 5;
    double* arr = new double[size];

    // fill it
    for (int i = 0; i < size; i++) {
        arr[i] = i * 1.5;
    }

    // print it
    std::cout << "Heap array: ";
    for (int i = 0; i < size; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";

    // delete array -- must use delete[] not delete!
    delete[] arr;
    std::cout << "array memory freed!\n";

    // ── PART 4: Memory leak example ──────────────────────
    std::cout << "\n=== MEMORY LEAK (what NOT to do) ===\n";

    // this is a memory leak -- never do this!
    // int* leak = new int(42);
    // if we never delete leak, that memory is gone forever
    // until the program ends
    std::cout << "memory leak = new without delete\n";
    std::cout << "always pair new with delete!\n";

    return 0;
}