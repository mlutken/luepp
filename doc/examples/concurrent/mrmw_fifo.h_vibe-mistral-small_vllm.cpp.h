#ifndef LUE_CONCURRENT_MRMW_FIFO_HPP
#define LUE_CONCURRENT_MRMW_FIFO_HPP

#include <atomic>
#include <cstddef>
#include <optional>
#include <type_traits>

namespace lue::concurrent {

// Multiple reader, multiple writer lockless FIFO queue
// Uses atomic indices and per-slot atomic flags for synchronization
template<typename T, std::size_t Capacity>
requires(Capacity > 0)
class mrmw_fifo
{
public:
    mrmw_fifo() = default;
    ~mrmw_fifo() = default;

    // Disable copying
    mrmw_fifo(mrmw_fifo const&) = delete;
    mrmw_fifo& operator=(mrmw_fifo const&) = delete;

    // Allow moving
    mrmw_fifo(mrmw_fifo&&) = default;
    mrmw_fifo& operator=(mrmw_fifo&&) = default;

    // Try to push a value into the queue
    // Returns true if the value was successfully pushed, false otherwise
    [[nodiscard]] bool try_push(T const& value)
    {
        return try_push_impl(value);
    }

    [[nodiscard]] bool try_push(T&& value)
    {
        return try_push_impl(std::move(value));
    }

    // Try to pop a value from the queue
    // Returns an optional containing the value if successful, nullopt otherwise
    [[nodiscard]] std::optional<T> try_pop()
    {
        std::optional<T> result;
        if (try_pop_impl(result))
        {
            return result;
        }
        return std::nullopt;
    }

    // Check if the queue is empty
    [[nodiscard]] bool empty() const noexcept
    {
        return read_index_.load(std::memory_order_acquire) == write_index_.load(std::memory_order_acquire);
    }

    // Check if the queue is full
    [[nodiscard]] bool full() const noexcept
    {
        // Calculate next write index
        auto const write_idx = write_index_.load(std::memory_order_acquire);
        auto const next_write_idx = (write_idx + 1) % Capacity;
        auto const read_idx = read_index_.load(std::memory_order_acquire);

        // Queue is full if next write would overwrite unread data
        return next_write_idx == read_idx;
    }

    // Get the current capacity of the queue
    [[nodiscard]] constexpr std::size_t capacity() const noexcept
    {
        return Capacity;
    }

    // Get the current size of the queue
    [[nodiscard]] std::size_t size() const noexcept
    {
        auto const write_idx = write_index_.load(std::memory_order_acquire);
        auto const read_idx = read_index_.load(std::memory_order_acquire);

        if (write_idx >= read_idx)
        {
            return write_idx - read_idx;
        }
        else
        {
            return Capacity - (read_idx - write_idx);
        }
    }

private:
    // Storage for queue elements
    T buffer_[Capacity];

    // Atomic indices for read and write positions
    alignas(64) std::atomic<std::size_t> read_index_{0};
    alignas(64) std::atomic<std::size_t> write_index_{0};

    // Atomic flags to track which slots are ready for reading
    alignas(64) std::atomic<bool> ready_[Capacity]{};

    // Helper function to push a value
    template<typename ValueType>
    [[nodiscard]] bool try_push_impl(ValueType&& value)
    {
        auto const write_idx = write_index_.load(std::memory_order_relaxed);
        auto const next_write_idx = (write_idx + 1) % Capacity;
        auto const read_idx = read_index_.load(std::memory_order_acquire);

        // Check if queue is full
        if (next_write_idx == read_idx)
        {
            return false;
        }

        // Store the value in the buffer
        buffer_[write_idx] = std::forward<ValueType>(value);

        // Mark the slot as ready for reading with release semantics
        ready_[write_idx].store(true, std::memory_order_release);

        // Update the write index with release semantics
        write_index_.store(next_write_idx, std::memory_order_release);

        return true;
    }

    // Helper function to pop a value
    [[nodiscard]] bool try_pop_impl(std::optional<T>& result)
    {
        auto const read_idx = read_index_.load(std::memory_order_relaxed);
        auto const write_idx = write_index_.load(std::memory_order_acquire);

        // Check if queue is empty
        if (read_idx == write_idx)
        {
            return false;
        }

        // Wait until the slot is ready for reading with acquire semantics
        if (!ready_[read_idx].load(std::memory_order_acquire))
        {
            return false;
        }

        // Load the value from the buffer
        result = std::move(buffer_[read_idx]);

        // Clear the ready flag for this slot with release semantics
        ready_[read_idx].store(false, std::memory_order_release);

        // Update the read index with release semantics
        read_index_.store((read_idx + 1) % Capacity, std::memory_order_release);

        return true;
    }
};

} // namespace lue::concurrent

#endif // LUE_CONCURRENT_MRMW_FIFO_HPP
