#pragma once

#include <atomic>
#include <cstddef>
#include <optional>
#include <stdexcept>
#include <type_traits>

namespace lue::concurrent {

// Multiple Reader, Multiple Writer Lockless FIFO
// Based on atomic indices and a circular buffer

template <typename T>
class mrmw_fifo {
public:
    explicit mrmw_fifo(std::size_t capacity)
        : capacity_{capacity}
        , buffer_{new T[capacity]}
        , read_index_{0}
        , write_index_{0}
    {
        // Ensure capacity is a power of two for efficient modulo using bitmask
        if (capacity == 0 || (capacity & (capacity - 1)) != 0) {
            throw std::invalid_argument("Capacity must be a power of two");
        }
    }

    ~mrmw_fifo()
    {
        delete[] buffer_.load(std::memory_order_relaxed);
    }

    // Disable copying
    mrmw_fifo(const mrmw_fifo&) = delete;
    mrmw_fifo& operator=(const mrmw_fifo&) = delete;

    // Allow moving
    mrmw_fifo(mrmw_fifo&& other) noexcept
        : capacity_{other.capacity_.load(std::memory_order_relaxed)}
        , buffer_{other.buffer_.load(std::memory_order_relaxed)}
        , read_index_{other.read_index_.load(std::memory_order_relaxed)}
        , write_index_{other.write_index_.load(std::memory_order_relaxed)}
    {
        other.capacity_.store(0, std::memory_order_relaxed);
        other.buffer_.store(nullptr, std::memory_order_relaxed);
        other.read_index_.store(0, std::memory_order_relaxed);
        other.write_index_.store(0, std::memory_order_relaxed);
    }

    mrmw_fifo& operator=(mrmw_fifo&& other) noexcept
    {
        if (this != &other) {
            delete[] buffer_.load(std::memory_order_relaxed);

            capacity_.store(other.capacity_.load(std::memory_order_relaxed), std::memory_order_relaxed);
            buffer_.store(other.buffer_.load(std::memory_order_relaxed), std::memory_order_relaxed);
            read_index_.store(other.read_index_.load(std::memory_order_relaxed), std::memory_order_relaxed);
            write_index_.store(other.write_index_.load(std::memory_order_relaxed), std::memory_order_relaxed);

            other.capacity_.store(0, std::memory_order_relaxed);
            other.buffer_.store(nullptr, std::memory_order_relaxed);
            other.read_index_.store(0, std::memory_order_relaxed);
            other.write_index_.store(0, std::memory_order_relaxed);
        }
        return *this;
    }

    // Try to push a value into the FIFO
    // Returns true if successful, false if FIFO is full
    bool try_push(const T& value)
    {
        return try_push_impl(value);
    }

    bool try_push(T&& value)
    {
        return try_push_impl(std::move(value));
    }

    // Blocking push (spin until space is available)
    void push(const T& value)
    {
        while (!try_push(value)) {
            // Busy wait
        }
    }

    void push(T&& value)
    {
        while (!try_push(std::move(value))) {
            // Busy wait
        }
    }

    // Try to pop a value from the FIFO
    // Returns std::nullopt if FIFO is empty
    std::optional<T> try_pop()
    {
        auto read_idx = read_index_.load(std::memory_order_acquire);
        auto write_idx = write_index_.load(std::memory_order_acquire);

        if (read_idx == write_idx) {
            return std::nullopt; // Empty
        }

        T* value = &buffer_[read_idx & (capacity_ - 1)];
        auto result = std::optional<T>(std::move(*value));

        read_index_.store(read_idx + 1, std::memory_order_release);
        return result;
    }

    // Blocking pop (spin until an item is available)
    T pop()
    {
        auto result = try_pop();
        while (!result) {
            result = try_pop();
        }
        return std::move(*result);
    }

    // Check if the FIFO is empty
    bool empty() const noexcept
    {
        return read_index_.load(std::memory_order_acquire) == write_index_.load(std::memory_order_acquire);
    }

    // Check if the FIFO is full
    bool full() const noexcept
    {
        auto read_idx = read_index_.load(std::memory_order_acquire);
        auto write_idx = write_index_.load(std::memory_order_acquire);
        return (write_idx - read_idx) >= capacity_;
    }

    // Get the current number of elements in the FIFO
    std::size_t size() const noexcept
    {
        auto read_idx = read_index_.load(std::memory_order_acquire);
        auto write_idx = write_index_.load(std::memory_order_acquire);
        return write_idx - read_idx;
    }

    // Get the maximum capacity of the FIFO
    std::size_t capacity() const noexcept
    {
        return capacity_.load(std::memory_order_relaxed);
    }

private:
    std::atomic<std::size_t> capacity_;
    std::atomic<T*> buffer_;

    alignas(64) std::atomic<std::size_t> read_index_;
    alignas(64) std::atomic<std::size_t> write_index_;

    template <typename U>
    bool try_push_impl(U&& value)
    {
        auto read_idx = read_index_.load(std::memory_order_acquire);
        auto write_idx = write_index_.load(std::memory_order_acquire);

        if ((write_idx - read_idx) >= capacity_) {
            return false; // Full
        }

        T* slot = &buffer_[write_idx & (capacity_ - 1)];
        *slot = std::forward<U>(value);

        write_index_.store(write_idx + 1, std::memory_order_release);
        return true;
    }
};

} // namespace lue::concurrent

