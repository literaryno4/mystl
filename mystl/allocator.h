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
 
#ifndef MYSTL_ALLOCATOR_H_
#define MYSTL_ALLOCATOR_H_

#include "construct.h"
#include "util.h"

namespace mystl {

template <class T>
class allocator {
   public:
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

   public:
    static T* allocate();
    static T* allocate(size_type n);
    static void deallocate(T* ptr);
    static void deallocate(T* ptr, size_type n);
    static void construct(T* ptr);
    static void construct(T* ptr, const T& value);
    static void construct(T* ptr, T&& value);
    template <class... Args>
    static void construct(T* ptr, Args&&... args);
    static void destroy(T* ptr);
    static void destroy(T* first, T* last);
};

template <class T>
T* allocator<T>::allocate() {
    return static_cast<T*>(::operator new(sizeof(T)));
}

template <class T>
T* allocator<T>::allocate(size_type n) {
    if (n == 0) {
        return nullptr;
    }
    return static_cast<T*>(::operator new(n * sizeof(T)));
}

template <class T>
void allocator<T>::deallocate(T* ptr) {
    if (ptr == nullptr) {
        return;
    }
    ::operator delete(ptr);
}

template <class T>
void allocator<T>::deallocate(T* ptr, size_type) {
    if (ptr == nullptr) {
        return;
    }
    ::operator delete(ptr);
}

template <class T>
void allocator<T>::construct(T* ptr) {
    mystl::construct(ptr);
}

template <class T>
void allocator<T>::construct(T* ptr, const T& value) {
    mystl::construct(ptr, value);
}

template <class T>
void allocator<T>::construct(T* ptr, T&& value) {
    mystl::construct(ptr, mystl::move(value));
}

template <class T>
template <class... Args>
void allocator<T>::construct(T* ptr, Args&&... args) {
    mystl::construct(ptr, mystl::forward<Args>(args)...);
}

template <class T>
void allocator<T>::destroy(T* ptr) {
    mystl::destroy(ptr);
}

template <class T>
void allocator<T>::destroy(T* first, T* last) {
    mystl::destroy(first, last);
}

}  // namespace mystl
#endif
