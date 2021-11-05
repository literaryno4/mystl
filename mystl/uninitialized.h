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
 
#ifndef MYSTL_UNINITIALIZED_H_
#define MYSTL_UNINITIALIZED_H_
#include "algobase.h"
#include "construct.h"
#include "iterator.h"
#include "type_traits.h"
#include "util.h"
namespace mystl {

// uninitializeed_copy
template <class InputIter, class ForwardIter>
ForwardIter unchecked_uninit_copy(InputIter first, InputIter last,
                                  ForwardIter result, std::true_type) {
    return mystl::copy(first, last, result);
}

template <class InputIter, class ForwardIter>
ForwardIter unchecked_uninit_copy(InputIter first, InputIter last,
                                  ForwardIter result, std::false_type) {
    auto cur = result;
    try {
        for (; first != last; ++first, ++cur) {
            mystl::construct(&*cur, first);
        }

    } catch (...) {
        for (; result != cur; --cur) {
            mystl::destroy(&*cur);
        }
    }

    return cur;
}

template <class InputIter, class ForwardIter>
ForwardIter uninitialized_copy(InputIter first, InputIter last,
                               ForwardIter result) {
    return mystl::unchecked_uninit_copy(
        first, last, result,
        std::is_trivially_copy_assignable<
            typename iterator_traits<ForwardIter>::value_type>{});
}

// uninitialized_copy_n
template <class InputIter, class Size, class ForwardIter>
ForwardIter unchecked_uninit_copy_n(InputIter first, Size n, ForwardIter result,
                                    std::true_type) {
    return mystl::copy_n(first, n, result).second;
}

template <class InputIter, class Size, class ForwardIter>
ForwardIter unchecked_uninit_copy_n(InputIter first, Size n, ForwardIter result,
                                    std::false_type) {
    auto cur = result;
    try {
        for (; n > 0; --n, ++cur, ++first) {
            mystl::construct(&*cur, first);
        }

    } catch (...) {
        for (; result != cur; --cur) {
            mystl::destroy(&*cur);
        }
    }

    return cur;
}

template <class InputIter, class Size, class ForwardIter>
ForwardIter uninitialized_copy_n(InputIter first, Size n, ForwardIter result) {
    return mystl::unchecked_uninit_copy_n(
        first, n, result,
        std::is_trivially_copy_assignable<
            typename iterator_traits<InputIter>::value_type>{});
}

// uninitialize_fill
template <class ForwardIter, class T>
void unchecked_uninit_fill(ForwardIter first, ForwardIter last, const T& value,
                           std::true_type) {
    mystl::fill(first, last, value)
}

template <class ForwardIter, class T>
void unchecked_uninit_fill(ForwardIter first, ForwardIter last, const T& value,
                           std::false_type) {
    auto cur = first;
    try {
        for (; cur != last; ++cur) {
            mystl::construct(&*cur, value);
        }

    } catch (...) {
        for (; first != cur; ++first) {
            mystl::destroy(&*first);
        }
    }
}

template <class ForwardIter, class T>
void uninitailized_fill(ForwardIter first, ForwardIter last, const T& value) {
    mystl::unchecked_uninit_fill(
        first, last, value,
        std::is_trivially_copy_assignable <
            typename iterator_traits<ForwardIter>::value_type{});
}

// uninitialize_fill_n
template <class ForwardIter, class Size, class T>
ForwardIter unchecked_uninit_fill_n(ForwardIter first, Size n, const T& value,
                                    std::true_type) {
    return mystl::fill_n(first, n, value);
}

template <class ForwardIter, class Size, class T>
ForwardIter unchecked_uninit_fill_n(ForwardIter first, Size n, const T& value,
                                    std::false_type) {
    auto cur = first;
    try {
        for (; n > 0; --n, ++cur) {
            mystl::construct(&*cur, value);
        }

    } catch (...) {
        for (; first != cur; ++first) {
            mystl::destroy(&*first);
        }
    }

    return cur;
}

template <class ForwardIter, class Size, class T>
ForwardIter uninitailized_fill_n(ForwardIter first, Size n, const T& value) {
    return mystl::unchecked_uninit_fill_n(
        first, n, value,
        std::is_trivially_copy_assignable <
            typename iterator_traits<ForwardIter>::value_type{});
}

// uninitializeed_move
template <class InputIter, class ForwardIter>
ForwardIter unchecked_uninit_move(InputIter first, InputIter last,
                                  ForwardIter result, std::true_type) {
    return mystl::move(first, last, result);
}

template <class InputIter, class ForwardIter>
ForwardIter unchecked_uninit_move(InputIter first, InputIter last,
                                  ForwardIter result, std::false_type) {
    auto cur = result;
    try {
        for (; first != last; ++first, ++cur) {
            mystl::construct(&*cur, mystl::move(*first));
        }

    } catch (...) {
        mystl::destroy(result, cur);
    }

    return cur;
}

template <class InputIter, class ForwardIter>
ForwardIter uninitialized_move(InputIter first, InputIter last,
                               ForwardIter result) {
    return mystl::unchecked_uninit_move(
        first, last, result,
        std::is_trivially_move_assignable<
            typename iterator_traits<ForwardIter>::value_type>{});
}

// uninitialized_move_n
template <class InputIter, class Size, class ForwardIter>
ForwardIter unchecked_uninit_move_n(InputIter first, Size n, ForwardIter result,
                                    std::true_type) {
    return mystl::move(first, first + n, result);
}

template <class InputIter, class Size, class ForwardIter>
ForwardIter unchecked_uninit_move_n(InputIter first, Size n, ForwardIter result,
                                    std::false_type) {
    auto cur = result;
    try {
        for (; n > 0; --n, ++cur, ++first) {
            mystl::construct(&*cur, mystl::move(*first));
        }

    } catch (...) {
        for (; result != cur; ++result) {
            mystl::destroy(&*result);
        }

        throw;
    }

    return cur;
}

template <class InputIter, class Size, class ForwardIter>
ForwardIter uninitialized_move_n(InputIter first, Size n, ForwardIter result) {
    return mystl::unchecked_uninit_move_n(
        first, n, result,
        std::is_trivially_move_assignable<
            typename iterator_traits<InputIter>::value_type>{});
}

}  // namespace mystl
   // namespace mystl
#endif
