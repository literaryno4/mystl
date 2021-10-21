/*
 * Created on Wed Oct 20 2021
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

#ifndef MYSTL_ALGO_H
#define MYSTL_ALGO_H

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4244)
#endif

#include <cstddef>
#include <ctime>

#include "algobase.h"
#include "functional.h"
#include "heap_algo.h"
#include "memory.h"

namespace mystl {

template <class InputIter, class UnaryPredicate>
bool all_of(InputIter first, InputIter last, UnaryPredicate unary_pred) {
    for (; first != last; ++first) {
        if (!unary_pred(*fisrt)) {
            return false;
        }
    }
    return true;
}

template <class InputIter, class UnaryPredicate>
bool any_of(InputIter first, InputIter last, UnaryPredicate unary_pred) {
    for (; first != last; ++first) {
        if (unary_pred(*fisrt)) {
            return true;
        }
    }
    return false;
}

template <class InputIter, class UnaryPredicate>
bool none_of(InputIter first, InputIter last, UnaryPredicate unary_pred) {
    for (; first != last; ++first) {
        if (unary_pred(*fisrt)) {
            return false;
        }
    }
    return true;
}

template <class InputIter, class T>
size_t count(InputIter first, InputIter last, const T& value) {
    size_t n = 0;
    for (; first != last; ++first) {
        if (*first == value) {
            ++n;
        }
    }
    return n;
}

template <class InputIter, class UnaryPredicate>
size_t count(InputIter first, InputIter last, UnaryPredicate unary_pred) {
    size_t n = 0;
    for (; first != last; ++first) {
        if (unary_pred(*first)) {
            ++n;
        }
    }
    return n;
}

template <class InputIter, class T>
InputIter find(InputIter first, InputIter last, const T& value) {
    while (first != last && *first != value) {
        ++first;
    }
    return first;
}

template <class InputIter, class UnaryPredicate>
InputIter find_if(InputIter first, InputIter last, UnaryPredicate unary_pred) {
    while (first != last && !unary_pred(*first)) {
        ++first;
    }
    return first;
}

template <class InputIter, class UnaryPredicate>
InputIter find_if_not(InputIter first, InputIter last,
                      UnaryPredicate unary_pred) {
    while (first != last && unary_pred(*first)) {
        ++first;
    }
    return first;
}

template <class ForwardIter1, class ForwardIter2>
ForwardIter1 search(ForwardIter1 first1, ForwardIter1 last1,
                    ForwardIter2 first2, ForwardIter2 last2) {
    auto d1 = mystl::distance(first1, last1);
    auto d2 = mystl::distance(first2, last2);
    if (d1 < d2) {
        return last1;
    }

    auto current1 = first1;
    auto current2 = first2;
    while (current2 != last2) {
        if (*current1 == *current2) {
            ++current1;
            ++current2;
        } else {
            if (d1 == d2) {
                return last1;
            } else {
                current1 = ++first1;
                current2 = first2;
                --d1;
            }
        }
    }
    return first1;
}

template <class ForwardIter1, class ForwardIter2, class Compared>
ForwardIter1 search(ForwardIter1 first1, ForwardIter1 last1,
                    ForwardIter2 first2, ForwardIter2 last2, Compared comp) {
    auto d1 = mystl::distance(first1, last1);
    auto d2 = mystl::distance(first2, last2);
    if (d1 < d2) {
        return last1;
    }

    auto current1 = first1;
    auto current2 = first2;
    while (current2 != last2) {
        if (comp(*current1, *current2)) {
            ++current1;
            ++current2;
        } else {
            if (d1 == d2) {
                return last1;
            } else {
                current1 = ++first1;
                current2 = first2;
                --d1;
            }
        }
    }
    return first1;
}

template <class ForwardIter, class Size, class T>
ForwardIter serch_n(ForwardIter first, ForwardIter last, Size n,
                    const T& value) {
    if (n <= 0) {
        return first;
    } else {
        first = mystl::find(first, last, value);
        while (first != last) {
            auto m = n - 1;
            auto i = first;
            ++i;
            while (i != last && m != 0 && *i == value) {
                ++i;
                --m;
            }
            if (m == 0) {
                return first;
            } else {
                first = mystl::find(i, last, value);
            }
        }
        return last;
    }
}

template <class ForwardIter, class Size, class T, class Compared>
ForwardIter serch_n(ForwardIter first, ForwardIter last, Size n, const T& value,
                    Compared comp) {
    if (n <= 0) {
        return first;
    } else {
        while (first != last) {
            if (comp(*first, value)) {
                break;
            }
            ++first;
        }
        while (first != last) {
            auto m = n - 1;
            auto i = first;
            ++i;
            while (i != last && m != 0 && comp(*i, value)) {
                ++i;
                --m;
            }
            if (m == 0) {
                return first;
            } else {
                first = mystl::find(i, last, value);
            }
        }
        return last;
    }
}

// find_end
template <class ForwardIter1, class ForwardIter2>
ForwardIter1 find_end_dispatch(ForwardIter1 first1, ForwardIter1 last1,
                               ForwardIter2 first2, ForwardIter2 last2,
                               forward_iterator_tag, forward_iterator_tag) {
    if (first2 == last2) {
        return last1;
    } else {
        auto result = last1;
        while (true) {
            auto new_result = mystl::serch(first1, last1, first2, last2);
            if (new_result == last1) {
                return result;
            } else {
                result = new_result;
                first1 = new_result;
                ++first1;
            }
        }
    }
}

template <class BidirectionalIter1, class BidirectionalIter2>
BidirectionalIter1 find_end_dispatch(BidirectionalIter1 first1,
                                     BidirectionalIter1 last1,
                                     BidirectionalIter2 first2,
                                     BidirectionalIter2 last2,
                                     bidirectional_iterator_tag,
                                     bidirectional_iterator_tag) {
    typedef reverse_iterator<BidirectionalIter1> reviter1;
    typedef reverse_iterator<BidirectionalIter2> reviter2;
    reviter1 rlast1(first1);
    reviter2 rlast2(first2);
    reviter1 rresult =
        mystl::serch(reviter1(last1), rlast1, reviter2(last2), rlast2);
    if (rresult == rlast1) {
        return last1;
    } else {
        auto result = rresult.base();
        mystl::advance(result, -mystl::distance(first2, last2));
        return result;
    }
}

template <class ForwardIter1, class ForwardIter2>
ForwardIter1 find_end(ForwardIter1 first1, ForwardIter1 last1,
                      ForwardIter2 first2, ForwardIter2 last2) {
    typedef typename iterator_traits<ForwardIter1>::iterator_category Category1;
    typedef typename iterator_traits<ForwardIter2>::iterator_category Category2;
    return mystl::find_end_dispatch(first1, last1, first2, last2, Category1(), Category2());
}

template <class ForwardIter1, class ForwardIter2, class Compared>
ForwardIter1 find_end_dispatch(ForwardIter1 first1, ForwardIter1 last1,
                               ForwardIter2 first2, ForwardIter2 last2,
                               forward_iterator_tag, forward_iterator_tag, Compared comp) {
    if (first2 == last2) {
        return last1;
    } else {
        auto result = last1;
        while (true) {
            auto new_result = mystl::serch(first1, last1, first2, last2, comp);
            if (new_result == last1) {
                return result;
            } else {
                result = new_result;
                first1 = new_result;
                ++first1;
            }
        }
    }
}

template <class BidirectionalIter1, class BidirectionalIter2, class Compared>
BidirectionalIter1 find_end_dispatch(BidirectionalIter1 first1,
                                     BidirectionalIter1 last1,
                                     BidirectionalIter2 first2,
                                     BidirectionalIter2 last2,
                                     bidirectional_iterator_tag,
                                     bidirectional_iterator_tag, Compared comp) {
    typedef reverse_iterator<BidirectionalIter1> reviter1;
    typedef reverse_iterator<BidirectionalIter2> reviter2;
    reviter1 rlast1(first1);
    reviter2 rlast2(first2);
    reviter1 rresult =
        mystl::serch(reviter1(last1), rlast1, reviter2(last2), rlast2, comp);
    if (rresult == rlast1) {
        return last1;
    } else {
        auto result = rresult.base();
        mystl::advance(result, -mystl::distance(first2, last2));
        return result;
    }
}

template <class ForwardIter1, class ForwardIter2, class Compared>
ForwardIter1 find_end(ForwardIter1 first1, ForwardIter1 last1,
                      ForwardIter2 first2, ForwardIter2 last2, Compared comp) {
    typedef typename iterator_traits<ForwardIter1>::iterator_category Category1;
    typedef typename iterator_traits<ForwardIter2>::iterator_category Category2;
    return mystl::find_end_dispatch(first1, last1, first2, last2, Category1(), Category2(), comp);
}

// find_first_of
template <class InputIter, class ForwardIter>
InputIter find_first_of(InputIter first1, InputIter last1, ForwardIter first2, ForwardIter last2) {
    for (; first1 != last1; ++first1) {
        for (auto iter = first2; iter != last2; ++iter) {
            if (*first1 == *iter) {
                return first1;
            }
        }
    }
    return last1;
}

template <class InputIter, class ForwardIter, class Compared>
InputIter find_first_of(InputIter first1, InputIter last1, ForwardIter first2, ForwardIter last2, Compared comp) {
    for (; first1 != last1; ++first1) {
        for (auto iter = first2; iter != last2; ++iter) {
            if (comp(*first1, *iter)) {
                return first1;
            }
        }
    }
    return last1;
}

// for_each
template <class InputIter, class Function>
Function for_each(InputIter first, InputIter last, Function f) {
    for (; first != last; ++first) {
        f(*first);
    }
    return f;
}

// adjacent_find
template <class ForwardIter>
ForwardIter adjacent_find(ForwardIter first, ForwardIter last) {
    if (first == last) return last;
    auto next = first;
    while (++next != last) {
        if (*first == *next) {
            return first;
        }
        first = next;
    }
    return last;
}

template <class ForwardIter, class Compared>
ForwardIter adjacent_find(ForwardIter first, ForwardIter last, Compared comp) {
    if (first == last) return last;
    auto next = first;
    while (++next != last) {
        if (comp(*first, *next)) {
            return first;
        }
        first = next;
    }
    return last;
}

// lower_bound
template <class ForwardIter, class T>
ForwardIter lbound_dispatch(ForwardIter first, ForwardIter last, const T& value, forward_iterator_tag) {
    auto len = mystl::distance(first, last);
    auto half = len;
    ForwardIter middle;
    while (len > 0) {
        half = len >> 1;
        middle = first;
        mystl::advance(middle, half);
        if (*middle < value) {
            first = middle;
            ++first;
            len = len - half - 1;
        } else {
            len = half;
        }
    }
    return first;
}

template <class RandomIter, class T>
RandomIter lbound_dispatch(RandomIter first, RandomIter last, const T& value, forward_iterator_tag) {
    auto len = mystl::distance(first, last);
    auto half = len;
    ForwardIter middle;
    while (len > 0) {
        half = len >> 1;
        middle = first;
        mystl::advance(middle, half);
        if (*middle < value) {
            first = middle;
            ++first;
            len = len - half - 1;
        } else {
            len = half;
        }
    }
    return first;
}

}  // namespace mystl

#endif