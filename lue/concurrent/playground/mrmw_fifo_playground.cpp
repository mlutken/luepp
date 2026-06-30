#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <concurrent/mrmw_fifo.hpp>

using namespace std;

int main()
{
    cerr << "--- mrmw_fifo playground ---\n";

    // Demo 1: Basic single-threaded usage
    {
        cerr << "\n=== Demo 1: Basic usage ===\n";
        lue::mrmw_fifo<int> fifo(8);
        
        fifo.init_queue(8);
        cerr << "Initial size: " << fifo.size() << "\n";
        
        fifo.push(10);
        fifo.push(20);
        fifo.push(30);
        cerr << "After pushing 3 elements, size: " << fifo.size() << "\n";
        
        int value;
        if (fifo.try_pop(value)) {
            cerr << "Popped: " << value << "\n";
        }
        cerr << "After popping, size: " << fifo.size() << "\n";
    }

    // Demo 2: Multi-threaded producer-consumer
    {
        cerr << "\n=== Demo 2: Multi-threaded producer-consumer ===\n";
        constexpr size_t queue_capacity = 16;
        constexpr int num_items = 100;
        
        lue::mrmw_fifo<int> fifo(queue_capacity);
        atomic<int> items_produced{0};
        atomic<int> items_consumed{0};
        atomic<bool> done_producing{false};
        
        // Producer thread
        thread producer([&]() {
            for (int i = 0; i < num_items; ++i) {
                while (!fifo.try_push(i)) {
                    // Queue full, retry
                    this_thread::yield();
                }
                items_produced++;
            }
            done_producing = true;
        });
        
        // Consumer thread
        thread consumer([&]() {
            int value;
            while (!done_producing || !fifo.empty()) {
                if (fifo.try_pop(value)) {
                    items_consumed++;
                } else {
                    this_thread::yield();
                }
            }
        });
        
        producer.join();
        consumer.join();
        
        cerr << "Items produced: " << items_produced << "\n";
        cerr << "Items consumed: " << items_consumed << "\n";
        cerr << "Final queue size: " << fifo.size() << "\n";
    }

    // Demo 3: Multiple producers, multiple consumers
    {
        cerr << "\n=== Demo 3: Multiple producers, multiple consumers ===\n";
        constexpr size_t queue_capacity = 32;
        constexpr int items_per_producer = 50;
        constexpr int num_producers = 3;
        constexpr int num_consumers = 2;
        
        lue::mrmw_fifo<int> fifo(queue_capacity);
        atomic<int> total_produced{0};
        atomic<int> total_consumed{0};
        atomic<int> producers_done{0};
        
        // Producer threads
        vector<thread> producers;
        for (int p = 0; p < num_producers; ++p) {
            producers.emplace_back([&, p]() {
                for (int i = 0; i < items_per_producer; ++i) {
                    int value = p * 1000 + i;  // Unique value per producer
                    while (!fifo.try_push(value)) {
                        this_thread::yield();
                    }
                    total_produced++;
                }
                producers_done++;
            });
        }
        
        // Consumer threads
        vector<thread> consumers;
        for (int c = 0; c < num_consumers; ++c) {
            consumers.emplace_back([&]() {
                int value;
                while (producers_done < num_producers || !fifo.empty()) {
                    if (fifo.try_pop(value)) {
                        total_consumed++;
                    } else {
                        this_thread::yield();
                    }
                }
            });
        }
        
        for (auto& t : producers) t.join();
        for (auto& t : consumers) t.join();
        
        cerr << "Total produced: " << total_produced << "\n";
        cerr << "Total consumed: " << total_consumed << "\n";
        cerr << "Final queue size: " << fifo.size() << "\n";
    }

    // Demo 4: try_emplace and std::optional interface
    {
        cerr << "\n=== Demo 4: try_emplace and optional interface ===\n";
        lue::mrmw_fifo<string> fifo(4);
        
        // Use emplace to construct in-place
        fifo.try_emplace<string>("Hello");
        fifo.try_emplace<string>("World");
        
        // Use optional interface
        while (auto value = fifo.try_pop()) {
            cerr << "Popped: " << *value << "\n";
        }
    }

    cerr << "\n--- All demos completed ---\n";
    return 0;
}
