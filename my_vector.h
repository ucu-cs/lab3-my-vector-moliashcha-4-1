#ifndef MY_VECTOR_H
#define MY_VECTOR_H

#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>

template <typename T>
class my_vector {
private:
    T* data_;
    size_t capacity_;
    size_t size_;

    size_t calc_cap(size_t new_min_capacity) const {
        const size_t new_capacity = capacity_ ? capacity_ * 2 : 1;
        return std::max(new_capacity, new_min_capacity);
    }

public:

    my_vector() noexcept : data_(nullptr), capacity_(0), size_(0) {}

    explicit my_vector(size_t count, const T& value = T()) : data_(nullptr), capacity_(count), size_(count) {
        if (count > 0) {
            data_ = new T[count];
            try {
                for (size_t i = 0; i < count; ++i) {
                    data_[i] = value;
                }
            } catch (...) {
                delete[] data_;
                data_ = nullptr;
                capacity_ = 0;
                size_ = 0;
                throw;
            }
        }
    }

    template <typename InputIt,
              typename = std::enable_if_t<!std::is_integral_v<InputIt>>>
    my_vector(InputIt first, InputIt last) : data_(nullptr), capacity_(0), size_(0) {
        size_t count = 0;
        for (auto it = first; it != last; ++it) {
            ++count;
        }

        if (count > 0) {
            data_ = new T[count];
            capacity_ = count;

            try {
                auto it = first;
                for (size_t i = 0; it != last; ++i, ++it) {
                    data_[i] = *it;
                    ++size_;
                }
            } catch (...) {
                for (size_t i = 0; i < size_; ++i) {
                    data_[i].~T();
                }
                delete[] data_;
                data_ = nullptr;
                size_ = 0;
                capacity_ = 0;
                throw;
            }
        }
    }

    my_vector(std::initializer_list<T> ilist) : data_(nullptr), capacity_(ilist.size()), size_(0) {
        if (ilist.size() > 0) {
            data_ = new T[capacity_];

            try {
                auto it = ilist.begin();
                for (size_t i = 0; it != ilist.end(); ++i, ++it) {
                    data_[i] = *it;
                    ++size_;
                }
            } catch (...) {
                for (size_t i = 0; i < size_; ++i) {
                    data_[i].~T();
                }
                delete[] data_;
                data_ = nullptr;
                size_ = 0;
                capacity_ = 0;
                throw;
            }
        }
    }

    my_vector(const my_vector& other) : data_(nullptr), size_(0), capacity_(other.size_) {
        if (other.size_ > 0) {
            data_ = new T[other.size_];

            try {
                for (size_t i = 0; i < other.size_; ++i) {
                    data_[i] = other.data_[i];
                    ++size_;
                }
            } catch (...) {
                for (size_t i = 0; i < size_; ++i) {
                    data_[i].~T();
                }
                delete[] data_;
                data_ = nullptr;
                size_ = 0;
                capacity_ = 0;
                throw;
            }
        }
    }

    my_vector(my_vector&& other) noexcept
        : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    ~my_vector() {
        if (data_) {
            delete[] data_;
        }
    }

    my_vector& operator=(const my_vector& other) {
        if (this != &other) {
            my_vector temp(other);
            swap(temp);
        }
        return *this;
    }

    my_vector& operator=(my_vector&& other) noexcept {
        if (this != &other) {
            clear();
            delete[] data_;

            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;

            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }

    T& operator[](size_t idx) noexcept { return data_[idx]; }
    const T& operator[](size_t idx) const noexcept { return data_[idx]; }

    T& at(size_t idx) {
        if (idx >= size_) {
            throw std::out_of_range("my_vector::at: index out of range");
        }
        return data_[idx];
    }

    const T& at(size_t idx) const {
        if (idx >= size_) {
            throw std::out_of_range("my_vector::at: index out of range");
        }
        return data_[idx];
    }

    T& front() { return data_[0]; }
    const T& front() const { return data_[0]; }

    T& back() { return data_[size_ - 1]; }
    const T& back() const { return data_[size_ - 1]; }

    T* begin() noexcept { return data_; }
    const T* begin() const noexcept { return data_; }
    const T* cbegin() const noexcept { return data_; }

    T* end() noexcept { return data_ + size_; }
    const T* end() const noexcept { return data_ + size_; }
    const T* cend() const noexcept { return data_ + size_; }

    std::reverse_iterator<T*> rbegin() noexcept { return std::reverse_iterator<T*>(end()); }
    std::reverse_iterator<const T*> rbegin() const noexcept { return std::reverse_iterator<const T*>(end()); }
    std::reverse_iterator<const T*> crbegin() const noexcept { return std::reverse_iterator<const T*>(end()); }

    std::reverse_iterator<T*> rend() noexcept { return std::reverse_iterator<T*>(begin()); }
    std::reverse_iterator<const T*> rend() const noexcept { return std::reverse_iterator<const T*>(begin()); }
    std::reverse_iterator<const T*> crend() const noexcept { return std::reverse_iterator<const T*>(begin()); }

    bool is_empty() const noexcept { return size_ == 0; }
    size_t size() const noexcept { return size_; }
    size_t capacity() const noexcept { return capacity_; }

    void reserve(size_t new_cap) {
        if (new_cap <= capacity_) return;

        T* new_data = new T[new_cap];

        try {
            for (size_t i = 0; i < size_; ++i) {
                new_data[i] = std::move_if_noexcept(data_[i]);
            }
        } catch (...) {
            delete[] new_data;
            throw;
        }

        delete[] data_;
        data_ = new_data;
        capacity_ = new_cap;
    }

    void shrink_to_fit() {
        if (size_ < capacity_) {
            if (size_ == 0) {
                delete[] data_;
                data_ = nullptr;
                capacity_ = 0;
            } else {
                T* new_data = new T[size_];

                try {
                    for (size_t i = 0; i < size_; ++i) {
                        new_data[i] = std::move_if_noexcept(data_[i]);
                    }
                } catch (...) {
                    delete[] new_data;
                    throw;
                }

                delete[] data_;
                data_ = new_data;
                capacity_ = size_;
            }
        }
    }

    void clear() noexcept {
        for (size_t i = 0; i < size_; ++i) {
            data_[i].~T();
        }
        size_ = 0;
    }

    void resize(size_t count, const T& value = T()) {
        if (count > size_) {
            if (count > capacity_) {
                reserve(count);
            }

            for (size_t i = size_; i < count; ++i) {
                data_[i] = value;
            }
        } else if (count < size_) {
            for (size_t i = count; i < size_; ++i) {
                data_[i].~T();
            }
        }

        size_ = count;
    }

    void push_back(const T& value) {
        if (size_ >= capacity_) {
            T value_copy = value;
            reserve(calc_cap(size_ + 1));
            data_[size_++] = std::move(value_copy);
        } else {
            data_[size_++] = value;
        }
    }

    void push_back(T&& value) {
        if (size_ >= capacity_) {
            reserve(calc_cap(size_ + 1));
        }
        data_[size_++] = std::move(value);
    }

    void pop_back() {
        if (size_ > 0) {
            --size_;
            data_[size_].~T();
        }
    }

    template <typename... Args>
    T& emplace_back(Args&&... args) {
        if (size_ >= capacity_) {
            reserve(calc_cap(size_ + 1));
        }

        new (&data_[size_]) T(std::forward<Args>(args)...);
        return data_[size_++];
    }

    T* insert(const T* pos, const T& value) {
        const size_t index = pos - begin();
        if (index > size_) {
            throw std::out_of_range("my_vector::insert: iterator out of range");
        }

        if (size_ >= capacity_) {
            T value_copy = value;
            size_t new_capacity = calc_cap(size_ + 1);
            T* new_data = new T[new_capacity];

            try {
                for (size_t i = 0; i < index; ++i) {
                    new_data[i] = std::move_if_noexcept(data_[i]);
                }

                new_data[index] = value_copy;

                for (size_t i = index; i < size_; ++i) {
                    new_data[i + 1] = std::move_if_noexcept(data_[i]);
                }

                delete[] data_;
                data_ = new_data;
                capacity_ = new_capacity;
                size_++;

            } catch (...) {
                delete[] new_data;
                throw;
            }
        } else {
            for (size_t i = size_; i > index; --i) {
                data_[i] = std::move_if_noexcept(data_[i - 1]);
            }

            data_[index] = value;
            size_++;
        }

        return begin() + index;
    }

    T* insert(const T* pos, T&& value) {
        const size_t index = pos - begin();
        if (index > size_) {
            throw std::out_of_range("my_vector::insert: iterator out of range");
        }

        if (size_ >= capacity_) {
            size_t new_capacity = calc_cap(size_ + 1);
            T* new_data = new T[new_capacity];

            try {
                for (size_t i = 0; i < index; ++i) {
                    new_data[i] = std::move_if_noexcept(data_[i]);
                }

                new_data[index] = std::move(value);

                for (size_t i = index; i < size_; ++i) {
                    new_data[i + 1] = std::move_if_noexcept(data_[i]);
                }

                delete[] data_;
                data_ = new_data;
                capacity_ = new_capacity;
                size_++;

            } catch (...) {
                delete[] new_data;
                throw;
            }
        } else {
            for (size_t i = size_; i > index; --i) {
                data_[i] = std::move_if_noexcept(data_[i - 1]);
            }

            data_[index] = std::move(value);
            size_++;
        }

        return begin() + index;
    }

    template <typename InputIt,
              typename = std::enable_if_t<!std::is_integral_v<InputIt>>>
    T* insert(const T* pos, InputIt first, InputIt last) {
        const size_t index = pos - begin();
        if (index > size_) {
            throw std::out_of_range("my_vector::insert: iterator out of range");
        }

        size_t count = 0;
        for (auto it = first; it != last; ++it) {
            ++count;
        }

        if (count == 0) {
            return begin() + index;
        }

        if (size_ + count > capacity_) {
            size_t new_capacity = calc_cap(size_ + count);
            T* new_data = new T[new_capacity];

            try {
                for (size_t i = 0; i < index; ++i) {
                    new_data[i] = std::move_if_noexcept(data_[i]);
                }

                size_t i = index;
                for (auto it = first; it != last; ++it, ++i) {
                    new_data[i] = *it;
                }

                for (size_t i = index; i < size_; ++i) {
                    new_data[i + count] = std::move_if_noexcept(data_[i]);
                }

                delete[] data_;
                data_ = new_data;
                capacity_ = new_capacity;
                size_ += count;

            } catch (...) {
                delete[] new_data;
                throw;
            }
        } else {
            for (size_t i = size_ - 1; i >= index && i < size_; --i) {
                data_[i + count] = std::move_if_noexcept(data_[i]);
            }

            size_t i = index;
            for (auto it = first; it != last; ++it, ++i) {
                data_[i] = *it;
            }

            size_ += count;
        }

        return begin() + index;
    }

    T* erase(const T* pos) {
        return erase(pos, pos + 1);
    }

    T* erase(const T* first, const T* last) {
        const size_t start_index = first - begin();
        const size_t end_index = last - begin();

        if (start_index > size_ || end_index > size_ || start_index > end_index) {
            throw std::out_of_range("my_vector::erase: iterator out of range");
        }

        if (start_index == end_index) {
            return begin() + start_index;
        }

        const size_t count = end_index - start_index;
        for (size_t i = end_index; i < size_; ++i) {
            data_[i - count] = std::move_if_noexcept(data_[i]);
        }

        for (size_t i = size_ - count; i < size_; ++i) {
            data_[i].~T();
        }

        size_ -= count;
        return begin() + start_index;
    }

    void swap(my_vector& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

    bool operator==(const my_vector& other) const {
        if (size_ != other.size_) {
            return false;
        }

        for (size_t i = 0; i < size_; ++i) {
            if (!(data_[i] == other.data_[i])) {
                return false;
            }
        }

        return true;
    }

    bool operator!=(const my_vector& other) const {
        return !(*this == other);
    }

    bool operator<(const my_vector& other) const {
        return std::lexicographical_compare(begin(), end(), other.begin(), other.end());
    }

    bool operator<=(const my_vector& other) const {
        return !(other < *this);
    }

    bool operator>(const my_vector& other) const {
        return other < *this;
    }

    bool operator>=(const my_vector& other) const {
        return !(*this < other);
    }
};

template <typename T>
void swap(my_vector<T>& lhs, my_vector<T>& rhs) noexcept {
    lhs.swap(rhs);
}

#endif // MY_VECTOR_H