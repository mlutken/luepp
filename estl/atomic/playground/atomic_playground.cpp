#include <iostream>
#include <vector>
#include <array>
#include <atomic/atomic.hpp>
#include <atomic/atomic_use.hpp>

using namespace std;



int main()
{
    cerr << "--- atomic playground ---\n";
    estl_use::atomic<int> a(2);
//    std::atomic<int> my_atomic;

    cerr << "a: " << a << "\n";

    return 0;
}
