#ifndef VECTOR_H
#define VECTOR_H

#include <limits>
#include <memory>
#include <stdexcept>

template <class T>
class Allocator
{
public:
    using value_type = T;

    T *allocate(size_t n)
    {
        return static_cast<T*>(operator new[](n * sizeof(T)));
    }

    void deallocate(T* p, size_t n)
    {
        operator delete[](p, n * sizeof(T));
    }
};

template <class T>
class Iterator : public std::iterator<std::random_access_iterator_tag, T>
{
    T *ptr_;

public:
    using value_type = T;

    explicit Iterator(T* ptr) : ptr_(ptr) {}

    bool operator==(const Iterator<T>& other) const
    {
        return ptr_ == other.ptr_;
    }

    bool operator!=(const Iterator<T>& other) const
    {
        return !(*this == other);
    }

    T &operator*() const
    {
        return *ptr_;
    }

    Iterator &operator++()
    {
        ++ptr_;
        return *this;
    }

    Iterator &operator--()
    {
        --ptr_;
        return *this;
    }

    Iterator &operator++(int)
    {
        auto t = this;
        ++ptr_;
        return *t;
    }

    Iterator &operator--(int)
    {
        auto t = this;
        --ptr_;
        return *t;
    }

    Iterator &operator+(size_t n)
    {
        return Iterator(ptr_ + n);
    }

    Iterator &operator-(size_t n)
    {
        return Iterator(ptr_ - n);
    }

    Iterator &operator+=(size_t n)
    {
        ptr_ += n;
        return *this;
    }

    Iterator &operator-=(size_t n)
    {
        ptr_ -= n;
        return *this;
    }

    bool operator<(const Iterator& other) const
    {
        return ptr_ < other.ptr_;
    }

    bool operator>(const Iterator& other) const
    {
        return ptr_ > other.ptr_;
    }

    bool operator<=(const Iterator& other) const
    {
        return ptr_ <= other.ptr_;
    }

    bool operator>=(const Iterator& other) const
    {
        return ptr_ >= other.ptr_;
    }

    T *operator[](size_t idx)
    {
        return ptr_[idx];
    }
};

template<class T, class Alloc = std::allocator<T>>
class Vector
{
    Alloc alloc_;
    T* data_;
    size_t size_;
    size_t capacity_;
public:
    using value_type = T;
    using allocator_type = Alloc;
    using iterator = Iterator<T>;
    using reverse_iterator = std::reverse_iterator<iterator>;

    Vector(size_t count = 0) : data_(alloc_.allocate(count)), size_(count), capacity_(count)
    {
        for (size_t i = 0; i < count; ++i) {
            new(data_ + i) T();
        }
    }

    ~Vector() {
        clear();
        alloc_.deallocate(data_, capacity_);
    }

    iterator begin()
    {
        return iterator(data_);
    }

    iterator end()
    {
        return iterator(data_ + size_);
    }

    reverse_iterator rbegin()
    {
        return reverse_iterator(end());
    }

    reverse_iterator rend()
    {
        return reverse_iterator(begin());
    }

    void push_back(T&& value)
    {
        if (size_ >= capacity_) {
            reserve(2 * capacity_ + 1);
        }
        new(data_ + size_) T(value);
        ++size_;
    }

    void push_back(const T& value) {
        if (size_ >= capacity_) {
            reserve(2 * capacity_);
        }
        new(data_ + size_) T(value);
        ++size_;
    }

    void pop_back() {
        if (size_ > 0) {
            --size_;
            (data_ + size_)->~T();
        }
    }

    void resize(size_t newSize)
    {
        if (size_ < newSize) {
            reserve(newSize);
            for (size_t i = size_; i < newSize; ++i) {
                alloc_.construct(data_ + i);
            }
            size_ = newSize;
        } else if(size_ > newSize) {
            for (size_t i = newSize; i < size_; ++i) {
                (data_ + i)->~T();
            }
            size_ = newSize;
        }
    }

    size_t size() const
    {
        return size_;
    }

    bool empty() const
    {
        return (size_ == 0);
    }

    size_t capacity() const
    {
        return capacity_;
    }

    void reserve(size_t count)
    {
        if (count > capacity_) {
            T* ptr = alloc_.allocate(count);
            for (size_t i = 0; i < size_; ++i) {
                new(ptr + i) T(*(data_ + i));
                (data_ + i)->~T();
            }
            alloc_.deallocate(data_, capacity_);
            
            capacity_ = count;
            data_ = ptr;
        }
    }

    void clear()
    {
        for (size_t i = 0; i < size_; ++i) {
            (data_ + i)->~T();
        }
        size_ = 0;
    }

    T &operator[](size_t idx)
    {
        if (idx < size_) {
            return data_[idx];
        } else {
            throw std::out_of_range("");
        }
    }
    const T &operator[](size_t idx) const
    {
        if (idx < size_) {
            return data_[idx];
        } else {
            throw std::out_of_range("");
        }
    }
};

#endif