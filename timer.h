#ifndef TIME_CHECK_INCLUDE
#define TIME_CHECK_INCLUDE

#include <chrono>
#include <atomic>

/**
 * @brief Get the current time fenced object
 *
 * @return std::chrono::high_resolution_clock::time_point
 */
inline std::chrono::high_resolution_clock::time_point get_current_time_fenced()
{
    std::atomic_thread_fence(std::memory_order_seq_cst);
    auto res_time = std::chrono::high_resolution_clock::now();
    std::atomic_thread_fence(std::memory_order_seq_cst);
    return res_time;
}

template<class D>
inline long long to_us(const D &d)
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(d).count();
}

#endif // TIME_CHECK_INCLUDE