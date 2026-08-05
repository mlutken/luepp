#include <concurrent/mrmw_fifo.hpp>

#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

using namespace std;


int main()
{
    cerr << "--- mrmw_fifo playground ---\n";

    using my_fifo = lue::mrmw_fifo<int, 1024>;

    constexpr int num_writers = 4;
    constexpr int num_readers = 4;
    constexpr int items_per_writer = 10000;

    my_fifo fifo;
    atomic<int> pushed_count{0};
    atomic<int> popped_count{0};

    // --- Writers ---
    vector<thread> writers;
    writers.reserve(num_writers);

    for (int w = 0; w < num_writers; ++w) {
        writers.emplace_back([&, w]() {
            for (int i = 0; i < items_per_writer; ++i) {
                const int value = w * items_per_writer + i;
                while (!fifo.push(value)) {
                    this_thread::yield();
                }
                pushed_count.fetch_add(1, memory_order_relaxed);
            }
        });
    }

    // --- Readers ---
    vector<thread> readers;
    readers.reserve(num_readers);

    atomic<bool> stop{false};

    for (int r = 0; r < num_readers; ++r) {
        readers.emplace_back([&]() {
            while (!stop.load(memory_order_relaxed) || !fifo.empty()) {
                if (fifo.pop()) {
                    popped_count.fetch_add(1, memory_order_relaxed);
                } else {
                    this_thread::yield();
                }
            }
        });
    }

    cerr << " --- Wait for writers to finish ---\n";
    // --- Wait for writers to finish ---
    for (auto& t : writers) {
        t.join();
    }

    // --- Signal readers to drain and stop ---
    stop.store(true, memory_order_relaxed);

    cerr << " --- Wait for readers to finish ---\n";
    // --- Wait for readers to finish ---
    for (auto& t : readers) {
        t.join();
    }

    cerr << "pushed: " << pushed_count.load() << "\n";
    cerr << "popped: " << popped_count.load() << "\n";
    cerr << "remaining: " << fifo.size() << "\n";

    const int expected = num_writers * items_per_writer;
    if (pushed_count.load() == expected &&
        popped_count.load() == expected &&
        fifo.empty())
    {
        cerr << "OK: all items pushed and popped\n";
        return 0;
    } else {
        cerr << "FAIL: data loss or mismatch\n";
        return 1;
    }
}
