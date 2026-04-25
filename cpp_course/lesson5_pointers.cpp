#include <iostream>

// function that takes a COPY -- original not changed
// same as normal Python function
void add_ten_copy(double x) {
    x = x + 10;
    std::cout << "Inside copy function : " << x << "\n";
}

// function that takes a POINTER -- original IS changed
// like passing a mutable object in Python
void add_ten_pointer(double* p) {
    *p = *p + 10;
    std::cout << "Inside pointer function : " << *p << "\n";
}

int main() {

    // PART 1 -- basic pointer
    double a = 42.0;
    double* p = &a;   // p holds the address of a

    std::cout << "Value of a    : " << a  << "\n";
    std::cout << "Address of a  : " << p  << "\n";
    std::cout << "Value via *p  : " << *p << "\n";

    // change a through the pointer
    *p = 100.0;
    std::cout << "After *p=100, a is : " << a << "\n";

    std::cout << "\n";

    // PART 2 -- copy vs pointer in functions
    double b = 50.0;

    add_ten_copy(b);
    std::cout << "After copy function, b is : " << b << "\n";

    std::cout << "\n";

    add_ten_pointer(&b);
    std::cout << "After pointer function, b is : " << b << "\n";

    return 0;
}