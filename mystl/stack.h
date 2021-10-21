/*
 * Created on Thu Sep 30 2021
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

#ifndef MYSTL_STACK_H_
#define MYSTL_STACK_H_

// template class: stack(adapter)

#include "deque.h"

namespace mystl {

template <class T, class Container = mystl::deque<T>>
class stack {
   public:
    typedef Container container_type;
    typedef typename Container::value_type value_type;
    typedef typename Container::size_type size_type;
    typedef typename Container::reference reference;
    typedef typename Container::const_reference const_reference;

    static_assert(std::is_same<T, value_type>::value,
                  "the value_type should be same with T");

   private:
    container_type c_;

   public:
    stack() = default;
    explicit stack(size_type n) : c_(n) {}
    stack(size_type n, const value_type& value) : c_(n, value) {}
    template <class IIter>
    stack(IIter first, IIter last) : c_(first, last) {}
    stack(std::initializer_list<T> ilist) : c_(ilist.begin(), ilist.end()) {}
    stack(const Container& c) : c_(c) {}
    stack(Container&& c) noexcept(
        std::is_nothrow_move_constructible<Container>::value)
        : c_(mystl::move(c)) {}
    stack(const stack& rhs) : c_(rhs.c_) {}
    stack(stack&& rhs) noexcept(
        std::is_nothrow_move_constructible<Container>::value)
        : c_(mystl::move(rhs.c_)) {}
    stack& operator=(stack&& rhs) noexcept(
        std::is_nothrow_move_assignable<Container>::value) {
        c_ = mystl::move(rhs.c_);
        return *this;
    }
    stack& operator=(std::initializer_list<T> ilist) {
        c_ = ilist;
        return *this;
    }
    ~stack() = default;

    reference top() { return c_.back(); }
    const_reference top() const { return c_.back(); }

    bool empty() const noexcept { return c_.empty(); }
    size_type size() const noexcept { return c_.size(); }

    template <class... Args>
    void emplace(Args&&... args) {
        c_.emplace_back(mystl::forward<Args>(args)...);
    }

    void push(const value_type& value) { c_.push_back(value); }
    void push(value_type&& value) { c_.push_back(mystl::move(value)); }

    void pop() { c_.pop_back(); }
    void clear() {
        while (!empty()) {
            pop();
        }
    }

    void swap(stack& rhs) noexcept(noexcept(mystl::swap(c_, rhs.c_))) {
        mystl::swap(c_, rhs.c_);
    }

   public:
    friend bool operator==(const stack& lhs, const stack& rhs) {
        return lhs.c_ == rhs.c_;
    }
    friend bool operator<(const stack& lhs, const stack& rhs) {
        return lhs.c_ < rhs.c_;
    }
};

template <class T, class Container>
bool operator==(const stack<T, Container>& lhs,
                const stack<T, Container>& rhs) {
    return lhs == rhs;
}

template <class T, class Container>
bool operator<(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
    return lhs < rhs;
}

template <class T, class Container>
bool operator!=(const stack<T, Container>& lhs,
                const stack<T, Container>& rhs) {
    return !(lhs == rhs);
}

template <class T, class Container>
bool operator>(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
    return rhs < lhs;
}

template <class T, class Container>
bool operator<=(const stack<T, Container>& lhs,
                const stack<T, Container>& rhs) {
    return !(rhs < lhs);
}

template <class T, class Container>
bool operator>=(const stack<T, Container>& lhs,
                const stack<T, Container>& rhs) {
    return !(lhs < rhs);
}

template <class T, class Container>
void swap(stack<T, Container>& lhs,
          stack<T, Container>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
    lhs.swap(rhs);
}

}  // namespace mystl

#endif