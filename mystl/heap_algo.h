/*
 * Created on Fri Oct 08 2021
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

#ifndef MYSTLHEAP_ALGO_H
#define MYSTL_HEAP_ALGO_H

// heap algorithm: push_heap, pop_heap, sort_heap, make_heap

#include "iterator.h"

namespace mystl {

// push_heap
// push a new value at the end of heap, then maxify heap
template <class RandomIter, class Distance, class T>
void push_heap_aux(RandomIter first, Distance holeIndex, Distance topIndex,
                   T value) {
    auto parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && *(first + parent) < value) {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = value;
}

template <class RandomIter, class Distance>
void push_heap_d(RandomIter first, RandomIter last, Distance*) {
    mystl::push_heap_aux(first, (last - first) - 1, static_cast<Distance>(0),
                         *(last - 1));
}

template <class RandomIter>
void push_heap(RandomIter first, RandomIter last) {
    mystl::push_heap_d(first, last, distance_type(first));
}

// push_heap, overload version
// push a new value at the end of heap, then maxify heap
template <class RandomIter, class Distance, class T, class Compared>
void push_heap_aux(RandomIter first, Distance holeIndex, Distance topIndex,
                   T value, Compared comp) {
    auto parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(*(first + parent), value)) {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = value;
}

template <class RandomIter, class Distance, class Compared>
void push_heap_d(RandomIter first, RandomIter last, Distance*, Compared comp) {
    mystl::push_heap_aux(first, (last - first) - 1, static_cast<Distance>(0),
                         *(last - 1), comp);
}

template <class RandomIter, class Compared>
void push_heap(RandomIter first, RandomIter last, Compared comp) {
    mystl::push_heap_d(first, last, distance_type(first), comp);
}

// pop_heap
// fetch root node and put it at the end of container, adjust heap
template <class RandomIter, class T, class Distance>
void adjust_heap(RandomIter first, Distance htoleIndex, Distance len, T value) {
    auto topIndex = htoleIndex;
    auto rchild = 2 * htoleIndex + 2;
    while (rchile < len) {
        if (*(first + rchild) < *(first + rchild - 1)) {
            --rchild;
        }
        *(first + htoleIndex) = *(first + rchild);
        htoleIndex = rchild;
        rchild = 2 * (rchild + 1);
    }
    if (rchild = len) {
        *(first + htoleIndex) = *(first + (rchild - 1));
        htoleIndex = rchild - 1;
    }
    mystl::push_heap_aux(first, htoleIndex, topIndex, value);
}

template <class RandomIter, class T, class Distance>
void pop_heap_aux(RandomIter first, RandomIter last, RandomIter result, T value,
                  Distance*) {
    *result = *first;
    mystl::adjust_heap(first, static_cast<Distance>(0), last - first, value);
}

template <class RandomIter>
void pop_heap(RandomIter first, RandomIter last) {
    mystl::pop_heap_aux(first, last - 1, last - 1, *(last - 1),
                        distance_type(first));
}

// pop_heap overload comp version
template <class RandomIter, class T, class Distance, class Compared>
void adjust_heap(RandomIter first, Distance holeIndex, Distance len, T value,
                 Compared comp) {
    auto topIndex = holeIndex;
    auto rchild = 2 * holeIndex + 2;
    while (rchild < len) {
        if (comp(*(first + rchild), *(first + rchild - 1))) --rchild;
        *(first + holeIndex) = *(first + rchild);
        holeIndex = rchild;
        rchild = 2 * (rchild + 1);
    }
    if (rchild == len) {
        *(first + holeIndex) = *(first + (rchild - 1));
        holeIndex = rchild - 1;
    }
    mystl::push_heap_aux(first, holeIndex, topIndex, value, comp);
}

template <class RandomIter, class T, class Distance, class Compared>
void pop_heap_aux(RandomIter first, RandomIter last, RandomIter result, T value,
                  Distance*, Compared comp) {
    *result = *first;
    mystl::adjust_heap(first, static_cast<Distance>(0), last - first, value,
                       comp);
}

template <class RandomIter, class Compared>
void pop_heap(RandomIter first, RandomIter last, Compared comp) {
    mystl::pop_heap_aux(first, last - 1, last - 1, *(last - 1),
                        distance_type(first), comp);
}

// sort_heap
template <class RandomIter>
void sort_heap(RandomIter first, RandomIter last) {
    while (last - first > 1) {
        mystl::pop_heap(first, last--);
    }
}

template <class RandomIter, class Compared>
void sort_heap(RandomIter first, RandomIter last, Compared comp) {
    while (last - first > 1) {
        mystl::pop_heap(first, last--, comp);
    }
}

// make_heap
template <class RandomIter, class Distance>
void make_heap_aux(RandomIter first, RandomIter last, Distance*) {
    if (last - first < 2) {
        return;
    }
    auto len = last - first;
    auto holeIndex = (len - 2) / 2;
    while (true) {
        mystl::adjust_heap(first, holeIndex, len, *(first + holeIndex));
        if (holeIndex == 0) {
            return;
        }
        holeIndex--;
    }
}

template <class RandomIter>
void make_heap(RandomIter first, RandomIter last) {
    mystl::make_heap_aux(first, last, distance_type(first));
}

// make_heap overload comp version
template <class RandomIter, class Distance, class Compared>
void make_heap_aux(RandomIter first, RandomIter last, Distance*,
                   Compared comp) {
    if (last - first < 2) {
        return;
    }
    auto len = last - first;
    auto holeIndex = (len - 2) / 2;
    while (true) {
        mystl::adjust_heap(first, holeIndex, len, *(first + holeIndex), comp);
        if (holeIndex == 0) {
            return;
        }
        holeIndex--;
    }
}

template <class RandomIter, class Compared>
void make_heap(RandomIter first, RandomIter last, Compared comp) {
    mystl::make_heap_aux(first, last, distance_type(first), comp);
}

}  // namespace mystl
#endif