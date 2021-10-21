/*
 * Created on Wed Sep 29 2021
 *
 * The MIT License (MIT)
 * Copyright (c) 2021 Chao Shu
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
 
#ifndef MYSTL_MEMORY_H_
#define MYSTL_MEMORY_H_

#include <climits>
#include <cstddef>
#include <cstdlib>

#include "algobase.h"
#include "allocator.h"
#include "construct.h"
#include "uninitialized.h"

namespace mystl {

template <class Tp>
constexpr Tp* address_of(Tp& value) noexcept {
    return &value;
}

// get / release temporary buffer
template <class T>
pair<T*, ptrdiff_t> get_buffer_helper(ptrdiff_t len, T*) {
    if (len > static_cast<ptrdiff_t>(INT_MAX / sizeof(T))) {
        len = INT_MAX / sizeof(T);
    }
    while (len > 0) {
        /* code */
        T* tmp = static_cast<T*>(malloc(static_cast<size_t>(len) * sizeof(T)));
        if (tmp) {
            return pair<T*, ptrdiff_t>(tmp, len);
        }
        len /= 2;
    }
    return pair<T*, ptrdiff_t>(nullptr, 0);
}

template <class T>
pair<T*, ptrdiff_t> get_temporary_buffer(ptrdiff_t len) {
    return get_buffer_helper(len, static_cast<T*>(0));
}

template <class T>
pair<T*, ptrdiff_t> get_temporary_buffer(ptrdiff_t len, T*) {
    return get_buffer_helper(len, static_cast<T*>(0));
}

template <class T>
void release_temporary_buffer(T* ptr) {
    free(ptr);
}

// template class: temporary_buffer
template <class ForwardIterator, class T>
class temporary_buffer {
   private:
    ptrdiff_t original_len;
    ptrdiff_t len;
    T* buffer;

   public:
    temporary_buffer(ForwardIterator first, ForwardIterator last);
    ~temporary_buffer() {
        mystl::destroy(buffer, buffer + len);
        free(buffer);
    }

   public:
    ptrdiff_t size() const noexcept { return len; }
    ptrdiff_t requested_size() const noexcept { return original_len; }
    T* begin() noexcept { return buffer; }
    T* end() noexcept { return buffer + len; }

   private:
    void allocate_buffer();
    void initialize_buffer(const T&, std::true_type) {}
    void initialize_buffer(const T&, value, std::false_type) {
        mystl::uninitailized_fill_n(buffer, len, value);
    }

   private:
    temporary_buffer(const temporary_buffer&);
    void operator=(const temporary_buffer&);
};

template <class ForwardIterator, class T>
temporary_buffer<ForwardIterator, T>::temporary_buffer(ForwardIterator first,
                                                       ForwardIterator last) {
    try {
        len = mystl::distance(first, last);
        allocate_buffer();
        if (len > 0) {
            initialize_buffer(*first,
                              std::is_trivially_default_constructible<T>());
        }
    } catch (...) {
        free(buffer);
        buffer = nullptr;
        len = 0;
    }
}

template <class ForwardIterator, class T>
void temporary_buffer<ForwardIterator, T>::allocate_buffer() {
    original_len = len;
    if (len > static_cast<ptrdiff_t>(INT_MAX / sizeof(T))) {
        len = INT_MAX / sizeof(T);
    }
    while (len > 0) {
        buffer = static_cast<T*>(malloc(len * sizeof(T)));
        if (buffer) {
            break;
        }
        len /= 2;
    }
}

// auto_ptr
template <class T>
class auto_ptr {
   public:
    typedef T elem_type;

   private:
    T* m_ptr;

   public:
    explicit auto_ptr(T* p = nullptr) : m_ptr(p) {}
    auto_ptr(auto_ptr& rhs) : m_ptr(rhs.release()) {}
    template <class U>
    auto_ptr(auto_ptr<U>& rhs) : m_ptr(rhs.release()) {}

    auto_ptr& operator=(auto_ptr& rhs) {
        if (this != &rhs) {
            delete m_ptr;
            m_ptr = rhs.release();
        }
        return *this;
    }

    template <class U>
    auto_ptr& operator=(auto_ptr<U>& rhs) {
        if (this->get() != rhs.get()) {
            delete m_ptr;
            m_ptr = rhs.release();
        }
        return *this;
    }

    ~auto_ptr() { delete m_ptr; }

    public:
    T& operator*() const { return *m_ptr; }
    T* operator->() const { return m_ptr; }

    T* get() const { return m_ptr; }

    T* release() {
        T* tmp = m_ptr;
        m_ptr = nullptr;
        return tmp;
    }

    void reset(T* p = nullptr) {
        if (m_ptr != p) {
            delete m_ptr;
            m_ptr = p;
        }
    }
};

}  // namespace mystl

#endif  // MYSTL_MEMORY_H_
