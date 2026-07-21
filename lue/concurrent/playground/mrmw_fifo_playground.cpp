#include <iostream>
#include <concurrent/mrmw_fifo.hpp>
#include <thread>
#include <vector>

using namespace std;

int main()
{
    cerr << "--- mrmw_fifo playground ---\n";

    // Create a FIFO with capacity 8 (must be power of two)
    lue::concurrent::mrmw_fifo<int> fifo(8);

    cerr << "Initial fifo.size(): " << fifo.size() << "\n";
    cerr << "Initial fifo.capacity(): " << fifo.capacity() << "\n";

    // Single-threaded push and pop
    cerr << "\n--- Single-threaded demo ---\n";
    for (int i = 0; i < 5; ++i) {
        fifo.push(i);
    }
    cerr << "After pushing 5 items, fifo.size(): " << fifo.size() << "\n";

    for (int i = 0; i < 3; ++i) {
        cerr << "Popped: " << fifo.pop() << "\n";
    }
    cerr << "After popping 3 items, fifo.size(): " << fifo.size() << "\n";

    // Multi-threaded demo: 2 writers, 2 readers
    cerr << "\n--- Multi-threaded demo ---\n";
    atomic<int> next_value{0};
    atomic<int> received_count{0};

    vector<thread> threads;

    // 2 writer threads
    for (int i = 0; i < 2; ++i) {
        threads.emplace_back([&fifo, &next_value]() {
            for (int j = 0; j < 6; ++j) {
                int val = next_value.fetch_add(1, memory_order_relaxed);
                fifo.push(val);
                cerr << "Writer pushed: " << val << "\n";
            }
        });
    }

    // 2 reader threads
    for (int i = 0; i < 2; ++i) {
        threads.emplace_back([&fifo, &received_count]() {
            for (int j = 0; j < 6; ++j) {
                int val = fifo.pop();
                cerr << "Reader popped: " << val << "\n";
                received_count.fetch_add(1, memory_order_relaxed);
            }
        });
    }

    // Wait for all threads to finish
    for (auto& t : threads) {
        t.join();
    }

    cerr << "\nAll threads finished. fifo.size(): " << fifo.size() << "\n";
    cerr << "Total items received: " << received_count.load() << "\n";

    return 0;
}