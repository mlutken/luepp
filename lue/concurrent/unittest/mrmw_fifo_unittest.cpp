#include <gtest/gtest.h>

#include <concurrent/mrmw_fifo.hpp>

#include <atomic>
#include <thread>
#include <vector>

using namespace testing;
using namespace lue::concurrent;


class MRMWFifoUnitTest : public testing::Test
{
};


TEST(MRMWFifoUnitTest, constructor_throws_if_capacity_not_power_of_two)
{
    EXPECT_THROW({
        mrmw_fifo<int> fifo(3);
    }, std::invalid_argument);

    EXPECT_NO_THROW({
        mrmw_fifo<int> fifo(4);
    });
}

TEST(MRMWFifoUnitTest, empty_and_full_on_empty_fifo)
{
    mrmw_fifo<int> fifo(4);
    EXPECT_TRUE(fifo.empty());
    EXPECT_FALSE(fifo.full());
    EXPECT_EQ(fifo.size(), 0);
}

TEST(MRMWFifoUnitTest, try_push_and_try_pop_single_threaded)
{
    mrmw_fifo<int> fifo(4);
    EXPECT_TRUE(fifo.try_push(1));
    EXPECT_TRUE(fifo.try_push(2));
    EXPECT_TRUE(fifo.try_push(3));
    EXPECT_TRUE(fifo.try_push(4));
    EXPECT_FALSE(fifo.try_push(5));

    EXPECT_EQ(fifo.size(), 4);
    EXPECT_FALSE(fifo.empty());
    EXPECT_TRUE(fifo.full());

    auto val = fifo.try_pop();
    ASSERT_TRUE(val.has_value());
    EXPECT_EQ(*val, 1);

    val = fifo.try_pop();
    ASSERT_TRUE(val.has_value());
    EXPECT_EQ(*val, 2);

    val = fifo.try_pop();
    ASSERT_TRUE(val.has_value());
    EXPECT_EQ(*val, 3);

    val = fifo.try_pop();
    ASSERT_TRUE(val.has_value());
    EXPECT_EQ(*val, 4);

    EXPECT_TRUE(fifo.empty());
    EXPECT_FALSE(fifo.full());
    EXPECT_EQ(fifo.size(), 0);

    val = fifo.try_pop();
    EXPECT_FALSE(val.has_value());
}

TEST(MRMWFifoUnitTest, push_and_pop_single_threaded)
{
    mrmw_fifo<int> fifo(8);
    for (int i = 0; i < 8; ++i) {
        fifo.push(i);
    }
    EXPECT_TRUE(fifo.full());

    for (int i = 0; i < 8; ++i) {
        EXPECT_EQ(fifo.pop(), i);
    }
    EXPECT_TRUE(fifo.empty());
}

TEST(MRMWFifoUnitTest, multiple_readers_single_writer)
{
    mrmw_fifo<int> fifo(16);
    std::vector<std::thread> readers;
    std::atomic<int> sum{0};
    std::atomic<int> received_count{0};

    // Start 3 reader threads
    for (int i = 0; i < 3; ++i) {
        readers.emplace_back([&fifo, &sum, &received_count]() {
            for (int j = 0; j < 5; ++j) {
                auto val = fifo.pop();
                sum.fetch_add(val, std::memory_order_relaxed);
                received_count.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }

    // Single writer thread
    std::thread writer([&fifo]() {
        for (int i = 0; i < 15; ++i) {
            fifo.push(i);
        }
    });

    writer.join();
    for (auto& reader : readers) {
        reader.join();
    }

    EXPECT_EQ(received_count.load(), 15);
    EXPECT_EQ(sum.load(), 105); // 0+1+...+14 = 105
}

TEST(MRMWFifoUnitTest, single_reader_multiple_writers)
{
    mrmw_fifo<size_t> fifo(32);
    std::vector<std::thread> writers;
    std::atomic<size_t> next_value{0};
    std::atomic<size_t> received_count{0};

    // Start 4 writer threads
    for (size_t i = 0; i < 4; ++i) {
        writers.emplace_back([&fifo, &next_value, &received_count]() {
            for (int j = 0; j < 8; ++j) {
                size_t val = next_value.fetch_add(1, std::memory_order_relaxed);
                fifo.push(val);
                received_count.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }

    // Single reader thread
    std::vector<size_t> received_values;
    std::thread reader([&fifo, &received_values, &received_count]() {
        for (size_t i = 0; i < 32; ++i) {
            received_values.push_back(fifo.pop());
        }
    });

    for (auto& writer : writers) {
        writer.join();
    }
    reader.join();

    EXPECT_EQ(received_count.load(), 32);
    EXPECT_EQ(received_values.size(), 32);

    // Check that all values from 0 to 31 are present
    std::sort(received_values.begin(), received_values.end());
    for (size_t i = 0; i < 32; ++i) {
        EXPECT_EQ(received_values[i], i);
    }
}

TEST(MRMWFifoUnitTest, move_constructor_and_move_assignment)
{
    mrmw_fifo<int> fifo1(4);
    fifo1.push(1);
    fifo1.push(2);

    // Move constructor
    mrmw_fifo<int> fifo2(std::move(fifo1));
    EXPECT_TRUE(fifo1.empty());
    EXPECT_EQ(fifo2.size(), 2);

    EXPECT_EQ(fifo2.pop(), 1);
    EXPECT_EQ(fifo2.pop(), 2);

    // Move assignment
    mrmw_fifo<int> fifo3(8);
    fifo3 = std::move(fifo2);
    EXPECT_TRUE(fifo2.empty());
    EXPECT_EQ(fifo3.size(), 0);
}

TEST(MRMWFifoUnitTest, capacity_and_size)
{
    mrmw_fifo<int> fifo(8);
    EXPECT_EQ(fifo.capacity(), 8);

    for (int i = 0; i < 5; ++i) {
        fifo.push(i);
        EXPECT_EQ(fifo.size(), i + 1);
    }

    for (int i = 0; i < 3; ++i) {
        fifo.pop();
        EXPECT_EQ(fifo.size(), 4 - i);
    }
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

