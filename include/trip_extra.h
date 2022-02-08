/*******************************************************************************
 * Copyright (c) 2019-2021 Craig Jacobson
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
 * @file trip_extra.h
 * @author Craig Jacobson
 * @brief TRiP imperative shell.
 */
#ifndef TRIP_EXTRA_H_
#define TRIP_EXTRA_H_
#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>

#include "trip_util.h"


void
trip_global_init();
Trip
trip_global_get();
void
trip_global_destroy();

Trip
trip_new();
void
trip_free(Trip *);

uint64_t
trip_now_ms(void);


#ifdef __cplusplus
}
#endif
#endif /* TRIP_EXTRA_H_ */

