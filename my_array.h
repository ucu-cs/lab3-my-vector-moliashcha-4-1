#ifndef MY_ARRAY_H
#define MY_ARRAY_H

#include <iostream>
#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <algorithm>

template <typename T, size_t N>
class my_array {
private:
    T data_[N];
public:
    constexpr my_array() = default;
    my_array(std::initializer_list<T> ilist) {
        auto it = ilist.begin();
        for (size_t i = 0; i < N && it != ilist.end(); ++i, ++it) {
            data_[i] = *it;
        }
    };

    template<typename InIt>
    my_array(InIt first, InIt last) {
        auto it = first;
        for (size_t i = 0; i < N && it != last; ++i, ++it) {
            data_[i] = *it;
        }
    }

    constexpr my_array(const my_array& other) = default;
    constexpr ~my_array() = default;

    my_array& operator=(const my_array& other) = default;
    auto operator<=>(const my_array&) const = default;

    // access without bounds checking
    T& operator[](size_t idx) { return data_[idx]; }
    const T& operator[](size_t idx) const { return data_[idx]; }

    T* begin() { return data_; }
    T* end() { return data_ + N; }

    const T* begin() const { return data_; }
    const T* end() const { return data_ + N; }

    const T* cbegin() const { return data_; }
    const T* cend() const { return data_ + N; }

    std::reverse_iterator<T*> rbegin() { return std::reverse_iterator<T*>(end()); }
    std::reverse_iterator<T*> rend() { return std::reverse_iterator<T*>(begin()); }

    const std::reverse_iterator<const T*> rbegin() const { return std::reverse_iterator<const T*>(end()); }
    const std::reverse_iterator<const T*> rend() const { return std::reverse_iterator<const T*>(begin()); }

    const std::reverse_iterator<const T*> crbegin() const { return std::reverse_iterator<const T*>(end()); }
    const std::reverse_iterator<const T*> crend() const { return std::reverse_iterator<const T*>(begin()); }

    // access with bounds checking
    T& at(size_t idx) {
        if (idx >= N) {
            throw std::out_of_range("my_array::at: index out of range");
        }
        return data_[idx];
    };

    const T& at(size_t idx) const {
        if (idx >= N) {
            throw std::out_of_range("my_array::at: index out of range");
        }
        return data_[idx];
    };

    constexpr bool is_empty() const { return N == 0; }
    constexpr size_t size() const { return N; }

    constexpr T* data() { return data_; }
    constexpr const T* data() const { return data_; }

    T& front() { return data_[0]; }
    const T& front() const { return data_[0]; }

    T& back() { return data_[N-1]; }
    const T& back() const { return data_[N-1]; }

    void fill(const T& value) { std::fill(begin(), end(), value); }

    void swap(my_array& other) noexcept {
        for (size_t i = 0; i < N; i++) {
            std::swap(data_[i], other.data_[i]);
        }
    }
};

#endif //MY_ARRAY_H
