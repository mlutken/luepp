#include <iostream>
#include <thread>
#include <vector>
#include <concurrent/mrmw_fifo.hpp>

using namespace std;

void producer(lue::concurrent::mrmw_fifo<int, 10>& fifo, int id, int count)
{
    for (int i = 0; i < count; ++i)
    {
        int value = id * 100 + i;
        while (!fifo.try_push(value))
        {
            // Queue is full, wait a bit
            this_thread::yield();
        }
        cerr << "Producer " << id << " pushed: " << value << "\n";
    }
}

void consumer(lue::concurrent::mrmw_fifo<int, 10>& fifo, int id, int count)
{
    int received = 0;
    while (received < count)
    {
        auto value = fifo.try_pop();
        if (value)
        {
            cerr << "Consumer " << id << " popped: " << *value << "\n";
            ++received;
        }
        else
        {
            // Queue is empty, wait a bit
            this_thread::yield();
        }
    }
}

int main()
{
    cerr << "--- mrmw_fifo playground ---\n";

    using my_fifo = lue::concurrent::mrmw_fifo<int, 10>;
    my_fifo fifo;

    cerr << "Initial fifo.size(): " << fifo.size() << "\n";
    cerr << "Initial fifo.empty(): " << (fifo.empty() ? "true" : "false") << "\n";
    cerr << "Initial fifo.full(): " << (fifo.full() ? "true" : "false") << "\n";

    // Test basic operations
    [[maybe_unused]] auto _ = fifo.try_push(10);
    [[maybe_unused]] auto __ = fifo.try_push(20);
    [[maybe_unused]] auto ___ = fifo.try_push(30);
    cerr << "After 3 pushes, fifo.size(): " << fifo.size() << "\n";

    auto val = fifo.try_pop();
    if (val)
    {
        cerr << "Popped: " << *val << " r\n";
    }
    cerr << "After 1 pop, fifo.size(): " << fifo.size() << "\n";

    // Test with multiple threads
    [[maybe_unused]] const int num_producers = 3;
    [[maybe_unused]] const int num_consumers = 2;
    [[maybe_unused]] const int items_per_producer = 5;

    vector<thread> producers;
    vector<thread> consumers;

    for (int i = 0; i < num_producers; ++i)
    {
        producers.emplace_back(producer, ref(fifo), i, items_per_producer);
    }

    for (int i = 0; i < num_consumers; ++i)
    {
        consumers.emplace_back(consumer, ref(fifo), i, items_per_producer);
    }

    // Wait for all producers to finish
    for (auto& p : producers)
    {
        p.join();
    }

    // Wait for all consumers to finish
    for (auto& c : consumers)
    {
        c.join();
    }

    cerr << "All producers and consumers finished.\n";
    cerr << "Final fifo.size(): " << fifo.size() << "\n";
    cerr << "Final fifo.empty(): " << (fifo.empty() ? "true" : "false") << "\n";

    return 0;
}
