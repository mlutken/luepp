#include <gtest/gtest.h>
#include <concurrent/mrmw_fifo.hpp>
#include <thread>
#include <vector>

using namespace testing;

class MRMWFifoUnitTest : public testing::Test
{
};

// ------------------------------
// --- PUBLIC functions tests ---
// ------------------------------
using int_fifo = lue::concurrent::mrmw_fifo<int, 10>;

TEST_F(MRMWFifoUnitTest, test_basic)
{
    int_fifo f;
    EXPECT_EQ(static_cast<size_t>(0u), f.size());
    EXPECT_EQ(10u, f.capacity());
    EXPECT_EQ(true, f.empty());
    EXPECT_EQ(false, f.full());
    {
        bool ok = f.try_push(10);
        EXPECT_EQ(true, ok);
        EXPECT_EQ(1u, f.size());
        EXPECT_EQ(false, f.empty());
        EXPECT_EQ(false, f.full());
    }
    {
        bool ok = f.try_push(20);
        EXPECT_EQ(true, ok);
        EXPECT_EQ(2u, f.size());
        EXPECT_EQ(false, f.empty());
        EXPECT_EQ(false, f.full());
    }
    {
        bool ok = f.try_push(30);
        EXPECT_EQ(true, ok);
        EXPECT_EQ(3u, f.size());
        EXPECT_EQ(false, f.empty());
        EXPECT_EQ(false, f.full());
    }
    {
        bool ok = f.try_push(40);
        EXPECT_EQ(true, ok);
        EXPECT_EQ(4u, f.size());
        EXPECT_EQ(false, f.empty());
        EXPECT_EQ(false, f.full());
    }
    {
        bool ok = f.try_push(50);
        EXPECT_EQ(true, ok);
        EXPECT_EQ(5u, f.size());
        EXPECT_EQ(false, f.empty());
        EXPECT_EQ(false, f.full());
    }
    {
        bool ok = f.try_push(60);
        EXPECT_EQ(true, ok);
        EXPECT_EQ(6u, f.size());
        EXPECT_EQ(false, f.empty());
        EXPECT_EQ(false, f.full());
    }
    {
        bool ok = f.try_push(70);
        EXPECT_EQ(true, ok);
        EXPECT_EQ(7u, f.size());
        EXPECT_EQ(false, f.empty());
        EXPECT_EQ(false, f.full());
    }
    {
        bool ok = f.try_push(80);
        EXPECT_EQ(true, ok);
        EXPECT_EQ(8u, f.size());
        EXPECT_EQ(false, f.empty());
        EXPECT_EQ(false, f.full());
    }
    {
        bool ok = f.try_push(90);
        EXPECT_EQ(true, ok);
        EXPECT_EQ(9u, f.size());
        EXPECT_EQ(false, f.empty());
        EXPECT_EQ(false, f.full());
    }
    {
        bool ok = f.try_push(100);
        EXPECT_EQ(true, ok);
        EXPECT_EQ(10u, f.size());
        EXPECT_EQ(false, f.empty());
        EXPECT_EQ(true, f.full());
    }
    {
        bool ok = f.try_push(110);
        EXPECT_EQ(false, ok);
        EXPECT_EQ(10u, f.size());
        EXPECT_EQ(false, f.empty());
        EXPECT_EQ(true, f.full());
    }
    {
        auto val = f.try_pop();
        EXPECT_TRUE(val.has_value());
        EXPECT_EQ(10, *val);
        EXPECT_EQ(9u, f.size());
        EXPECT_EQ(false, f.empty());
        EXPECT_EQ(false, f.full());
    }
    {
        auto val = f.try_pop();
        EXPECT_TRUE(val.has_value());
        EXPECT_EQ(20, *val);
        EXPECT_EQ(8u, f.size());
        EXPECT_EQ(false, f.empty());
        EXPECT_EQ(false, f.full());
    }
}

TEST_F(MRMWFifoUnitTest, test_size)
{
    int_fifo f;
    EXPECT_EQ(static_cast<size_t>(0u), f.size());
    [[maybe_unused]] bool ok = f.try_push(10);
    EXPECT_EQ(1u, f.size());
    [[maybe_unused]] auto _ = f.try_push(20);
    EXPECT_EQ(2u, f.size());
    [[maybe_unused]] auto __ = f.try_push(30);
    EXPECT_EQ(3u, f.size());
    [[maybe_unused]] auto ___ = f.try_push(40);
    EXPECT_EQ(4u, f.size());
    [[maybe_unused]] auto ____ = f.try_push(50);
    EXPECT_EQ(5u, f.size());
    [[maybe_unused]] auto _____ = f.try_push(60);
    EXPECT_EQ(6u, f.size());
    [[maybe_unused]] auto ______ = f.try_push(70);
    EXPECT_EQ(7u, f.size());
    [[maybe_unused]] auto _______ = f.try_push(80);
    EXPECT_EQ(8u, f.size());
    [[maybe_unused]] auto ________ = f.try_push(90);
    EXPECT_EQ(9u, f.size());
    [[maybe_unused]] auto _________ = f.try_push(100);
    EXPECT_EQ(10u, f.size());
    
    auto val = f.try_pop();
    EXPECT_TRUE(val.has_value());
    EXPECT_EQ(9u, f.size());
    
    [[maybe_unused]] auto __________ = f.try_push(110);
    EXPECT_EQ(10u, f.size());
    
    val = f.try_pop();
    EXPECT_TRUE(val.has_value());
    EXPECT_EQ(9u, f.size());
    val = f.try_pop();
    EXPECT_TRUE(val.has_value());
    EXPECT_EQ(8u, f.size());
    val = f.try_pop();
    EXPECT_TRUE(val.has_value());
    EXPECT_EQ(7u, f.size());
    val = f.try_pop();
    EXPECT_TRUE(val.has_value());
    EXPECT_EQ(6u, f.size());
    val = f.try_pop();
    EXPECT_TRUE(val.has_value());
    EXPECT_EQ(5u, f.size());
    val = f.try_pop();
    EXPECT_TRUE(val.has_value());
    EXPECT_EQ(4u, f.size());
    val = f.try_pop();
    EXPECT_TRUE(val.has_value());
    EXPECT_EQ(3u, f.size());
    val = f.try_pop();
    EXPECT_TRUE(val.has_value());
    EXPECT_EQ(2u, f.size());
    val = f.try_pop();
    EXPECT_TRUE(val.has_value());
    EXPECT_EQ(1u, f.size());
    val = f.try_pop();
    EXPECT_TRUE(val.has_value());
    EXPECT_EQ(static_cast<size_t>(0u), f.size());
}

TEST_F(MRMWFifoUnitTest, test_empty_full)
{
    int_fifo f;
    EXPECT_TRUE(f.empty());
    EXPECT_FALSE(f.full());
    
    // Fill the queue
    for (int i = 0; i < 10; ++i)
    {
        [[maybe_unused]] bool ok = f.try_push(i);
    }
    
    EXPECT_FALSE(f.empty());
    EXPECT_TRUE(f.full());
    
    // Empty the queue
    for (int i = 0; i < 10; ++i)
    {
        auto val = f.try_pop();
        EXPECT_TRUE(val.has_value());
    }
    
    EXPECT_TRUE(f.empty());
    EXPECT_FALSE(f.full());
}

void producer_thread(lue::concurrent::mrmw_fifo<int, 100>& fifo, int start, int count)
{
    for (int i = 0; i < count; ++i)
    {
        int value = start + i;
        while (!fifo.try_push(value))
        {
            // Queue is full, wait a bit
            std::this_thread::yield();
        }
    }
}

void consumer_thread(lue::concurrent::mrmw_fifo<int, 100>& fifo, int expected_count)
{
    int received = 0;
    while (received < expected_count)
    {
        auto value = fifo.try_pop();
        if (value)
        {
            ++received;
        }
        else
        {
            // Queue is empty, wait a bit
            std::this_thread::yield();
        }
    }
}

TEST_F(MRMWFifoUnitTest, test_multithreaded)
{
    lue::concurrent::mrmw_fifo<int, 100> fifo;
    
    [[maybe_unused]] const int num_producers = 5;
    [[maybe_unused]] const int num_consumers = 3;
    [[maybe_unused]] const int items_per_producer = 20;
    
    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;
    
    // Start producers
    for (int i = 0; i < num_producers; ++i)
    {
        producers.emplace_back(producer_thread, std::ref(fifo), i * 1000, items_per_producer);
    }
    
    // Start consumers
    for (int i = 0; i < num_consumers; ++i)
    {
        consumers.emplace_back(consumer_thread, std::ref(fifo), items_per_producer);
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
    
    // Verify queue is empty
    EXPECT_TRUE(fifo.empty());
    EXPECT_EQ(static_cast<size_t>(0u), fifo.size());
}

TEST_F(MRMWFifoUnitTest, test_different_types)
{
    // Test with double
    lue::concurrent::mrmw_fifo<double, 5> double_fifo;
    [[maybe_unused]] bool ok = double_fifo.try_push(3.14);
    [[maybe_unused]] auto _ = double_fifo.try_push(2.71);
    EXPECT_EQ(2u, double_fifo.size());
    
    auto dval = double_fifo.try_pop();
    EXPECT_TRUE(dval.has_value());
    EXPECT_DOUBLE_EQ(3.14, *dval);
    
    // Test with string
    lue::concurrent::mrmw_fifo<std::string, 3> string_fifo;
    [[maybe_unused]] auto __ = string_fifo.try_push(std::string("hello"));
    [[maybe_unused]] auto ___ = string_fifo.try_push(std::string("world"));
    EXPECT_EQ(2u, string_fifo.size());
    
    auto sval = string_fifo.try_pop();
    EXPECT_TRUE(sval.has_value());
    EXPECT_EQ(std::string("hello"), *sval);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
