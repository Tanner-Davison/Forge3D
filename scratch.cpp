#include <iostream>
#include <print>

int main() {
    const char* names[] = {"Tanner", "Rebecca", "Hello"};
    for (const auto name : names) {
        std::print(" {},", name);
    }
    return 0;
}
