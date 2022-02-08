/*******************************************************************************
 * Copyright (c) 2019 Craig Jacobson
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
 ******************************************************************************/
/**
 * @file trip_util.h
 * @author Craig Jacobson
 * @brief Utilities that can be swapped out.
 */
#ifndef TRIP_UTIL_H_
#define TRIP_UTIL_H_
#ifdef __cplusplus
extern "C" {
#endif


#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <sodium.h>


#define tripm_alloc   _tripm_alloc_impl
#define tripm_realloc _tripm_realloc_impl
#define tripm_free    _tripm_free_impl
#define tripm_zero    _tripm_zero_impl

#ifndef LIKELY
#ifdef __GNUC__
#define LIKELY(x)   __builtin_expect(!!(x), 1)
#else
#define LIKELY(x)   (x)
#endif
#endif

#ifndef UNLIKELY
#ifdef __GNUC__
#define UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#define UNLIKELY(x) (x)
#endif
#endif

#ifndef INLINE
#ifdef __GNUC__
#define INLINE __attribute__((always_inline)) inline
#else
#define INLINE
#endif
#endif

#ifndef NOINLINE
#ifdef __GNUC__
#define NOINLINE __attribute__((noinline))
#else
#define NOINLINE
#endif
#endif


void
_tripm_zero_impl(void *, size_t);


#ifdef __cplusplus
}
#endif
#endif /* TRIP_UTIL_H_ */

