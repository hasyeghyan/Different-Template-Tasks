#include <iostream>
#include <type_traits>

template <typename T>
T&& forward(std::remove_reference_t<T>& arg) {
    return static_cast<T&&>(arg);
}

template <typename T>
T&& forward(std::remove_reference_t<T>&& arg) {
    static_assert(!std::is_lvalue_reference_v<T>, "Don't do this");
    return static_cast<T&&>(arg);
}

void g(int& x) {
    std::cout << "Lvalue" << std::endl;
}

void g(int&& x) {
    std::cout << "Rvalue" << std::endl;
}

template <typename T>
void foo(T&& arg) {
    g(forward<T>(arg));
}

int main() {
      
    int x = 7;
    foo(x);
    foo(10);
    foo(std::move(x));

    return 0;
};