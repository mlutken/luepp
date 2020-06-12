#include <iostream>
#include <vector>
#include <concurrent/srsw_fifo.hpp>
#include <concurrent/srsw_fifo_s.hpp>

using namespace std;


int main()
{
    cerr << "--- srsw_fifo playground ---\n";

//    using my_fifo = estl::srsw_fifo<int>;
    using my_fifo = estl::srsw_fifo_s<int,4>;

//    my_fifo f(4);
    my_fifo f;

    cerr << "Initial f.size(): " << f.size() << "\n";
    f.push(10); // size = 1
    f.push(20); // size = 2
    f.push(30); // size = 3
    f.pop();    // size = 2
    f.push(40); // size = 3
    const auto sz = f.size();

    cerr << "f.size(): " << sz << "\n";

    return 0;
}
