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
 * @file trip_protocol.h
 * @author Craig Jacobson
 * @brief TRiP protocol specific values and utilities.
 */
#ifndef TRIP_PROTOCOL_H_
#define TRIP_PROTOCOL_H_
#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>


#define TRIPP_CTRL_LEN (1)
#define TRIPP_ID_LEN (8)
#define TRIPP_SEQ_LEN (4)

#define TRIPP_SEG_MIN (TRIPP_CTRL_LEN + TRIPP_ID_LEN + TRIPP_SEQ_LEN)

enum trip_control_e
{
    TRIPP_CTRL_STREAM,
    TRIPP_CTRL_OPEN,
    TRIPP_CTRL_MAX,
};

#define TRIPP_ENC_BIT ((uint8_t)0x80)


#ifdef __cplusplus
}
#endif
#endif /* TRIP_PROTOCOL_H_ */

