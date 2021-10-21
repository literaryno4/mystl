/*
 * Created on Mon Oct 18 2021
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

#ifndef MYSTL_NUMERIC_H
#define MYSTL_NUMERIC_H

#include "iterator.h"

namespace mystl {

template <class InputIter, class T>
T accumulate(InputIter first, InputIter last, T init) {
    for (; first != last; ++first) {
        init += *first;
    }
    return init;
}

template <class InputIter, class T, class BinaryOp>
T accumulate(InputIter first, InputIter last, T init, BinaryOp binary_op) {
    for (; first != last; ++first) {
        init = binary_op(init, *first);
    }
    return init;
}

template <class InputIter, class OutputIter>
OutputIter adjacent_difference(InputIter first, InputIter last,
                               OutputIter result) {
    if (first == last) {
        return result;
    }
    *result = *first;
    auto value = *first;
    while (++first != last) {
        auto tmp = *first;
        *++result = tmp - value;
        value = tmp;
    }
    return ++result;
}

template <class InputIter, class OutputIter, class BinaryOp>
OutputIter adjacent_difference(InputIter first, InputIter last,
                               OutputIter result, BinaryOp binary_op) {
    if (first == last) {
        return result;
    }
    *result = *first;
    auto value = *first;
    while (++first != last) {
        auto tmp = *first;
        *++result = binary_op(tmp, value);
        value = tmp;
    }
    return ++result;
}

template <class InputIter1, class InputIter2, class T>
T inner_product(InputIter1 first1, InputIter1 last1, InputIter2 first2,
                InputIter2 last2, T init) {
    for (; first1 != last1; ++first1, ++first2) {
        init = init + (*first1 * *first2);
    }
    return init;
}

template <class InputIter1, class InputIter2, class T, class BinaryOp1,
          class BinaryOp2>
T inner_product(InputIter1 first1, InputIter1 last1, InputIter2 first2,
                InputIter2 last2, T init, BinaryOp1 binary_op1,
                BinaryOp2 binary_op2) {
    for (; first1 != last1; ++first1, ++first2) {
        init = binary_op1(init, binary_op2(*first1, *first2));
    }
    return init;
}

template <class ForwardIter, class T>
void iota(ForwardIter first, ForwardIter last, T value) {
    while (first != last) {
        *first++ = value;
        ++value;
    }
}

template <class InputIter, class OutputIter>
OutputIter partial_sum(InputIter first, InputIter last, OutputIter result) {
    if (first == last) return result;
    *result = *first;
    auto value = *first;
    while (++first != last) {
        value = value + *first;
        *++result = value;
    }
    return ++result;
}

template <class InputIter, class OutputIter, class BinaryOp>
OutputIter partial_sum(InputIter first, InputIter last, OutputIter result,
                       BinaryOp binary_op) {
    if (first == last) return result;
    *result = *first;
    auto value = *first;
    while (++first != last) {
        value = binary_op(value, *first);
        *++result = value;
    }
    return ++result;
}

}  // namespace mystl

#endif