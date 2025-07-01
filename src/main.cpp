#include <iostream>
#include <cstdint> 

int main() {
    std::int_fast8_t w1 { 4 };
    std::int_fast8_t w2 { sizeof(int) };
    std::int_fast8_t w3 {};
    std::cout << "Enter the weight of the third item: ";
    std::cin >> w3;
    std::cout << "The sum of the weights is: " << w1 + w2 + w3 << std::endl;
    return 0;
}