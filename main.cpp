#include <iostream>
#include <map>
#include <utility>

#include "MyAllocator.hpp"
#include "MyContainer.hpp"

long long factorial(int n) {
    long long r = 1;
    for (int i = 1; i <= n; ++i)
        r *= i;
    return r;
}

int main() {
    std::cout << "std::map со стандартным аллокатором\n";
    std::map<int, int> m1;
    for (int i = 0; i < 10; ++i)
        m1[i] = factorial(i);
    
    std::cout << "Вывод содержимого m1:\n";
    for (auto& [k, v] : m1)
        std::cout << " " << k << " " << v << "\n";
    

    std::cout << "std::map с пользовательским аллокатором\n";
    using Pair = std::pair<const int, int>;
    using Alloc = MyAllocator<Pair, 10>;
    std::map<int, int, std::less<int>, Alloc> m2;

    for (int i = 0; i < 10; ++i)
        m2[i] = factorial(i);

    std::cout << "Вывод содержимого m2:\n";
    for (auto& [k, v] : m2)
        std::cout << " " << k << " " << v << "\n";

    std::cout << "MyContainer со стандартным аллокатором\n";
    MyContainer<int> c1;
    for (int i = 0; i < 10; ++i)
        c1.push_back(i);

    std::cout << "Вывод содержимого c1:\n";
    std::cout << " ";
    for (int x : c1)
        std::cout << x << " ";
    std::cout << "\n";

    std::cout << "MyContainer с пользовательским аллокатором\n";
    MyContainer<int, MyAllocator<int, 10>> c2;
    for (int i = 0; i < 10; ++i)
        c2.push_back(i);

    std::cout << "Вывод содержимого c2:\n";
    std::cout << " ";
    for (int x : c2)
        std::cout << x << " ";
    std::cout << "\n";

    return 0;
}