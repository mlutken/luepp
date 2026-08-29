#include <gtest/gtest.h>
#include <concurrent/mrmw_fifo.hpp>
#include <thread>
#include <vector>
#include <atomic>
#include <string>
#include <memory>

using namespace testing;

class MRMWFifoUnitTest : public testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

// ------------------------------
// --- Basic type definitions ---
// ------------------------------
using int_fifo = lue::mrmw_fifo<int>;
using string_fifo = lue::mrmw_fifo<std::string>;

// ------------------------------
// --- Basic Operations Tests ---
// ------------------------------

TEST_F(MRMWFifoUnitTest, test_default_constructor)
{
    int_fifo f;
    EXPECT_EQ(0u, f.capacity());
    EXPECT_EQ(0u, f.size());
    EXPECT_TRUE(f.empty());
}

TEST_F(MRMWFifoUnitTest, test_constructor_with_capacity)
{
    int_fifo f(16);
    EXPECT_EQ(16u, f.capacity());
    EXPECT_EQ(0u, f.size());
    EXPECT_TRUE(f.empty());
    EXPECT_FALSE(f.full());
}

TEST_F(MRMWFifoUnitTest, test_capacity_rounded_to_power_of_two)
{
    int_fifo f(10);
    EXPECT_EQ(16u, f.capacity());  // Should round up to next power of 2
    
    int_fifo f2(5);
    EXPECT_EQ(8u, f2.capacity());
    
    int_fifo f3(100);
    EXPECT_EQ(128u, f3.capacity());
}

TEST_F(MRMWFifoUnitTest, test_init_queue)
{
    int_fifo f;
    f.init_queue(8);
    EXPECT_EQ(8u, f.capacity());
    EXPECT_EQ(0u, f.size());
    EXPECT_TRUE(f.empty());
}

TEST_F(MRMWFifoUnitTest, test_basic_push_pop)
{
    int_fifo f(8);
    
    EXPECT_TRUE(f.push(10));
    EXPECT_EQ(1u, f.size());
    EXPECT_EQ(10, f.front());
    EXPECT_FALSE(f.empty());
    
    EXPECT_TRUE(f.push(20));
    EXPECT_EQ(2u, f.size());
    EXPECT_EQ(10, f.front());
    
    EXPECT_TRUE(f.push(30));
    EXPECT_EQ(3u, f.size());
    EXPECT_EQ(10, f.front());
    
    EXPECT_TRUE(f.pop());
    EXPECT_EQ(2u, f.size());
    EXPECT_EQ(20, f.front());
    
    EXPECT_TRUE(f.pop());
    EXPECT_EQ(1u, f.size());
    EXPECT_EQ(30, f.front());
    
    EXPECT_TRUE(f.pop());
    EXPECT_EQ(0u, f.size());
    EXPECT_TRUE(f.empty());
}

TEST_F(MRMWFifoUnitTest, test_queue_full)
{
    int_fifo f(4);  // Capacity will be 4
    
    // Note: Vyukov MPMC algorithm uses ALL capacity slots (no wasted slot)
    EXPECT_TRUE(f.push(1));
    EXPECT_TRUE(f.push(2));
    EXPECT_TRUE(f.push(3));
    EXPECT_EQ(3u, f.size());
    EXPECT_FALSE(f.full());  // Not full yet - one more slot available
    
    EXPECT_TRUE(f.push(4));  // 4th push succeeds - queue now full
    EXPECT_EQ(4u, f.size());
    EXPECT_TRUE(f.full());  // Now full
    
    EXPECT_FALSE(f.push(5));  // Should fail - queue is full
    EXPECT_EQ(4u, f.size());
}

TEST_F(MRMWFifoUnitTest, test_queue_empty)
{
    int_fifo f(8);
    
    EXPECT_TRUE(f.empty());
    EXPECT_FALSE(f.pop());  // Should fail - queue is empty
    
    int value;
    EXPECT_FALSE(f.try_pop(value));
}

TEST_F(MRMWFifoUnitTest, test_push_move)
{
    int_fifo f(8);
    
    int val = 42;
    EXPECT_TRUE(f.push(std::move(val)));
    EXPECT_EQ(1u, f.size());
    EXPECT_EQ(42, f.front());
}

TEST_F(MRMWFifoUnitTest, test_multiple_push_pop_cycles)
{
    int_fifo f(8);
    
    // First cycle
    for (int i = 0; i < 5; ++i) {
        EXPECT_TRUE(f.push(i));
    }
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(i, f.front());
        EXPECT_TRUE(f.pop());
    }
    EXPECT_TRUE(f.empty());
    
    // Second cycle
    for (int i = 100; i < 105; ++i) {
        EXPECT_TRUE(f.push(i));
    }
    for (int i = 100; i < 105; ++i) {
        EXPECT_EQ(i, f.front());
        EXPECT_TRUE(f.pop());
    }
    EXPECT_TRUE(f.empty());
}

// ------------------------------
// --- try_push/try_pop Tests ---
// ------------------------------

TEST_F(MRMWFifoUnitTest, test_try_push)
{
    int_fifo f(8);
    
    EXPECT_TRUE(f.try_push(10));
    EXPECT_TRUE(f.try_push(20));
    EXPECT_TRUE(f.try_push(30));
    EXPECT_EQ(3u, f.size());
}

TEST_F(MRMWFifoUnitTest, test_try_pop)
{
    int_fifo f(8);
    
    f.push(10);
    f.push(20);
    
    int value;
    EXPECT_TRUE(f.try_pop(value));
    EXPECT_EQ(10, value);
    EXPECT_EQ(1u, f.size());
    
    EXPECT_TRUE(f.try_pop(value));
    EXPECT_EQ(20, value);
    EXPECT_EQ(0u, f.size());
    
    EXPECT_FALSE(f.try_pop(value));  // Queue empty
}

TEST_F(MRMWFifoUnitTest, test_try_pop_optional)
{
    int_fifo f(8);
    
    f.push(10);
    f.push(20);
    
    auto val1 = f.try_pop();
    EXPECT_TRUE(val1.has_value());
    EXPECT_EQ(10, *val1);
    
    auto val2 = f.try_pop();
    EXPECT_TRUE(val2.has_value());
    EXPECT_EQ(20, *val2);
    
    auto val3 = f.try_pop();
    EXPECT_FALSE(val3.has_value());
}

TEST_F(MRMWFifoUnitTest, test_emplace)
{
    string_fifo f(8);
    
    EXPECT_TRUE(f.emplace<std::string>("Hello"));
    EXPECT_TRUE(f.emplace<std::string>("World"));
    EXPECT_EQ(2u, f.size());
    EXPECT_EQ("Hello", f.front());
    
    EXPECT_TRUE(f.pop());
    EXPECT_EQ("World", f.front());
}

TEST_F(MRMWFifoUnitTest, test_try_emplace)
{
    string_fifo f(8);
    
    EXPECT_TRUE(f.try_emplace<std::string>("Test"));
    EXPECT_TRUE(f.try_emplace<std::string>("Value"));
    EXPECT_EQ(2u, f.size());
    EXPECT_EQ("Test", f.front());
}

// ------------------------------
// --- Flush Test ---
// ------------------------------

TEST_F(MRMWFifoUnitTest, test_flush)
{
    int_fifo f(8);
    
    for (int i = 0; i < 5; ++i) {
        f.push(i);
    }
    EXPECT_EQ(5u, f.size());
    
    f.flush();
    EXPECT_EQ(0u, f.size());
    EXPECT_TRUE(f.empty());
}

// ------------------------------
// --- Swap Test ---
// ------------------------------

TEST_F(MRMWFifoUnitTest, test_swap)
{
    int_fifo f1(8);
    int_fifo f2(16);
    
    f1.push(10);
    f1.push(20);
    f2.push(30);
    f2.push(40);
    f2.push(50);
    
    f1.swap(f2);
    
    EXPECT_EQ(16u, f1.capacity());
    EXPECT_EQ(3u, f1.size());
    EXPECT_EQ(30, f1.front());
    
    EXPECT_EQ(8u, f2.capacity());
    EXPECT_EQ(2u, f2.size());
    EXPECT_EQ(10, f2.front());
}

// ------------------------------
// --- Resize Test ---
// ------------------------------

TEST_F(MRMWFifoUnitTest, test_resize_queue)
{
    int_fifo f(4);
    
    f.push(10);
    f.push(20);
    f.push(30);
    EXPECT_EQ(3u, f.size());
    EXPECT_EQ(4u, f.capacity());
    
    f.resize_queue(16);
    EXPECT_EQ(16u, f.capacity());
    EXPECT_EQ(3u, f.size());
    
    // Verify elements preserved
    EXPECT_EQ(10, f.front());
    EXPECT_TRUE(f.pop());
    EXPECT_EQ(20, f.front());
    EXPECT_TRUE(f.pop());
    EXPECT_EQ(30, f.front());
    EXPECT_TRUE(f.pop());
    EXPECT_TRUE(f.empty());
}

// ------------------------------
// --- Different Data Types ---
// ------------------------------

TEST_F(MRMWFifoUnitTest, test_string_type)
{
    string_fifo f(8);
    
    EXPECT_TRUE(f.push("Hello"));
    EXPECT_TRUE(f.push("World"));
    EXPECT_EQ(2u, f.size());
    EXPECT_EQ("Hello", f.front());
    
    std::string value;
    EXPECT_TRUE(f.try_pop(value));
    EXPECT_EQ("Hello", value);
    
    auto opt = f.try_pop();
    EXPECT_TRUE(opt.has_value());
    EXPECT_EQ("World", *opt);
}

TEST_F(MRMWFifoUnitTest, test_unique_ptr_type)
{
    lue::mrmw_fifo<std::unique_ptr<int>> f(8);
    
    auto ptr1 = std::make_unique<int>(42);
    auto ptr2 = std::make_unique<int>(100);
    
    EXPECT_TRUE(f.try_push(std::move(ptr1)));
    EXPECT_TRUE(f.try_push(std::move(ptr2)));
    EXPECT_EQ(2u, f.size());
    
    auto val1 = f.try_pop();
    EXPECT_TRUE(val1.has_value());
    EXPECT_EQ(42, **val1);
    
    auto val2 = f.try_pop();
    EXPECT_TRUE(val2.has_value());
    EXPECT_EQ(100, **val2);
}

struct TestStruct
{
    int x;
    double y;
    std::string z;
    
    TestStruct() : x(0), y(0.0) {}
    TestStruct(int a, double b, const std::string& c) : x(a), y(b), z(c) {}
};

TEST_F(MRMWFifoUnitTest, test_custom_type)
{
    lue::mrmw_fifo<TestStruct> f(8);
    
    EXPECT_TRUE(f.try_emplace(10, 3.14, "test"));
    EXPECT_TRUE(f.try_emplace(20, 2.71, "pi"));
    
    auto val = f.try_pop();
    EXPECT_TRUE(val.has_value());
    EXPECT_EQ(10, val->x);
    EXPECT_DOUBLE_EQ(3.14, val->y);
    EXPECT_EQ("test", val->z);
}

// ------------------------------
// --- Single-threaded Stress Test ---
// ------------------------------

TEST_F(MRMWFifoUnitTest, test_many_operations)
{
    // Capacity must be >= num_items since this is single-threaded
    // (no consumer emptying the queue while we push)
    int_fifo f(2048);
    
    const int num_items = 1000;
    
    // Push many items
    for (int i = 0; i < num_items; ++i) {
        EXPECT_TRUE(f.try_push(i)) << "Failed to push item " << i 
                                   << ", size=" << f.size() 
                                   << ", capacity=" << f.capacity();
    }
    
    EXPECT_EQ(static_cast<size_t>(num_items), f.size());
    
    // Pop all items
    for (int i = 0; i < num_items; ++i) {
        int value;
        EXPECT_TRUE(f.try_pop(value)) << "Failed to pop item " << i;
        EXPECT_EQ(i, value);
    }
    
    EXPECT_TRUE(f.empty());
}

// ------------------------------
// --- Multi-threaded Tests ---
// ------------------------------

TEST_F(MRMWFifoUnitTest, test_single_producer_single_consumer)
{
    int_fifo f(64);
    const int num_items = 1000;
    std::atomic<bool> done{false};
    std::atomic<int> consumed_count{0};
    
    // Producer thread
    std::thread producer([&f, num_items]() {
        for (int i = 0; i < num_items; ++i) {
            while (!f.try_push(i)) {
                std::this_thread::yield();
            }
        }
    });
    
    // Consumer thread
    std::thread consumer([&f, &done, &consumed_count, num_items]() {
        int prev = -1;
        while (consumed_count < num_items) {
            auto val = f.try_pop();
            if (val.has_value()) {
                EXPECT_EQ(prev + 1, *val);
                prev = *val;
                consumed_count++;
            } else {
                std::this_thread::yield();
            }
        }
    });
    
    producer.join();
    consumer.join();
    
    EXPECT_EQ(num_items, consumed_count.load());
    EXPECT_TRUE(f.empty());
}

TEST_F(MRMWFifoUnitTest, test_multiple_producers_multiple_consumers)
{
    int_fifo f(128);
    const int items_per_producer = 100;
    const int num_producers = 4;
    const int num_consumers = 3;
    const int total_items = items_per_producer * num_producers;
    
    std::atomic<int> produced_count{0};
    std::atomic<int> consumed_count{0};
    std::atomic<int> producers_done{0};
    
    // Track consumed values per producer
    std::vector<std::vector<bool>> consumed_values(num_producers, std::vector<bool>(items_per_producer, false));
    
    // Producer threads
    std::vector<std::thread> producers;
    for (int p = 0; p < num_producers; ++p) {
        producers.emplace_back([&f, p, items_per_producer, &produced_count, &producers_done]() {
            for (int i = 0; i < items_per_producer; ++i) {
                int value = p * 10000 + i;  // Unique value encoding producer ID
                while (!f.try_push(value)) {
                    std::this_thread::yield();
                }
                produced_count++;
            }
            producers_done++;
        });
    }
    
    // Consumer threads
    std::vector<std::thread> consumers;
    for (int c = 0; c < num_consumers; ++c) {
        consumers.emplace_back([&f, &consumed_count, &producers_done, num_producers, total_items]() {
            while (consumed_count < total_items) {
                auto val = f.try_pop();
                if (val.has_value()) {
                    consumed_count++;
                } else if (producers_done >= num_producers) {
                    // All producers done and queue appears empty
                    if (f.empty()) break;
                } else {
                    std::this_thread::yield();
                }
            }
        });
    }
    
    for (auto& t : producers) t.join();
    for (auto& t : consumers) t.join();
    
    EXPECT_EQ(total_items, produced_count.load());
    EXPECT_EQ(total_items, consumed_count.load());
    EXPECT_TRUE(f.empty());
}

TEST_F(MRMWFifoUnitTest, test_producer_burst)
{
    int_fifo f(32);
    const int burst_size = 50;
    const int num_bursts = 10;
    std::atomic<int> total_produced{0};
    std::atomic<int> total_consumed{0};
    std::atomic<bool> done{false};
    
    // Producer: push in bursts
    std::thread producer([&]() {
        for (int burst = 0; burst < num_bursts; ++burst) {
            for (int i = 0; i < burst_size; ++i) {
                while (!f.try_push(burst * 1000 + i)) {
                    std::this_thread::yield();
                }
                total_produced++;
            }
            // Small delay between bursts
            std::this_thread::sleep_for(std::chrono::microseconds(100));
        }
        done = true;
    });
    
    // Consumer: continuous consumption
    std::thread consumer([&]() {
        while (!done || !f.empty()) {
            if (f.try_pop()) {
                total_consumed++;
            } else {
                std::this_thread::yield();
            }
        }
    });
    
    producer.join();
    consumer.join();
    
    EXPECT_EQ(num_bursts * burst_size, total_produced.load());
    EXPECT_EQ(num_bursts * burst_size, total_consumed.load());
}

TEST_F(MRMWFifoUnitTest, test_high_contention)
{
    int_fifo f(16);  // Small capacity increases contention
    const int num_operations = 500;
    std::atomic<int> successful_pushes{0};
    std::atomic<int> successful_pops{0};
    
    // 4 producer threads
    std::vector<std::thread> producers;
    for (int p = 0; p < 4; ++p) {
        producers.emplace_back([&]() {
            for (int i = 0; i < num_operations; ++i) {
                if (f.try_push(i)) {
                    successful_pushes++;
                } else {
                    std::this_thread::yield();
                }
            }
        });
    }
    
    // 4 consumer threads
    std::vector<std::thread> consumers;
    for (int c = 0; c < 4; ++c) {
        consumers.emplace_back([&]() {
            for (int i = 0; i < num_operations; ++i) {
                if (f.try_pop()) {
                    successful_pops++;
                } else {
                    std::this_thread::yield();
                }
            }
        });
    }
    
    for (auto& t : producers) t.join();
    for (auto& t : consumers) t.join();
    
    // Drain remaining items
    while (f.try_pop()) {
        successful_pops++;
    }
    
    EXPECT_EQ(successful_pushes.load(), successful_pops.load());
}

// ------------------------------
// --- Edge Cases ---
// ------------------------------

TEST_F(MRMWFifoUnitTest, test_power_of_two_capacity)
{
    // Test various capacities to ensure power-of-2 rounding works
    EXPECT_EQ(2u, int_fifo(1).capacity());
    EXPECT_EQ(2u, int_fifo(2).capacity());
    EXPECT_EQ(4u, int_fifo(3).capacity());
    EXPECT_EQ(8u, int_fifo(5).capacity());
    EXPECT_EQ(8u, int_fifo(7).capacity());
    EXPECT_EQ(8u, int_fifo(8).capacity());
    EXPECT_EQ(16u, int_fifo(9).capacity());
    EXPECT_EQ(32u, int_fifo(31).capacity());
    EXPECT_EQ(128u, int_fifo(100).capacity());
    EXPECT_EQ(256u, int_fifo(200).capacity());
}

TEST_F(MRMWFifoUnitTest, test_size_accuracy)
{
    int_fifo f(16);
    
    EXPECT_EQ(0u, f.size());
    
    for (int i = 0; i < 10; ++i) {
        f.push(i);
    }
    EXPECT_EQ(10u, f.size());
    
    for (int i = 0; i < 5; ++i) {
        f.pop();
    }
    EXPECT_EQ(5u, f.size());
    
    for (int i = 0; i < 5; ++i) {
        f.push(i);
    }
    EXPECT_EQ(10u, f.size());
    
    f.flush();
    EXPECT_EQ(0u, f.size());
}

TEST_F(MRMWFifoUnitTest, test_wrap_around)
{
    int_fifo f(4);
    
    // Fill and empty multiple times to test index wrap-around
    // Note: Vyukov MPMC algorithm uses ALL capacity slots (no wasted slot)
    for (int cycle = 0; cycle < 100; ++cycle) {
        // Fill completely (4 items in capacity 4)
        for (int i = 0; i < 4; ++i) {
            EXPECT_TRUE(f.push(cycle * 10 + i));
        }
        EXPECT_TRUE(f.full());
        
        // Empty
        for (int i = 0; i < 4; ++i) {
            int expected = cycle * 10 + i;
            EXPECT_EQ(expected, f.front());
            EXPECT_TRUE(f.pop());
        }
        EXPECT_TRUE(f.empty());
    }
}

// ------------------------------
// --- Main ---
// ------------------------------

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
