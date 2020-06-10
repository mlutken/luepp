#include <iostream>
#include <vector>
#include <concurrent/srsw_fifo.hpp>

using namespace std;


int main()
{
    cerr << "--- srsw_fifo playground ---\n";

    using my_fifo = estl::srsw_fifo<int>;

    my_fifo f(5);

    cerr << "Initial f.size(): " << f.size() << "\n";
    f.push(2);
    cerr << "f.size(): " << f.size() << "\n";

    return 0;
}
