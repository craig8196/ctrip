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
 * @file trip.h
 * @author Craig Jacobson
 * @brief TRiP implementation.
 */
#ifndef TRIP_H_
#define TRIP_H_
#ifdef __cplusplus
extern "C" {
#endif


#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "hitime.h"


#define TRIP_VERSION_MAJOR (0)


/* TRiP Options */
enum trip_option_e
{
    TRIP_OPTION_MAXCONN,
    TRIP_OPTION_MAXSTREAM,
    TRIP_OPTION_MAXCREDIT,
};

/* TRiP Actions */
enum trip_action_e
{
    // FRAMEWORK DO
    TRIP_TIMEOUT,
    TRIP_RECV,
    TRIP_SEND_READY,
    TRIP_CONN,

    // USER DO
    TRIP_WAIT,
    TRIP_RECV_REJECT,
    TRIP_SEND,
    TRIP_MESSAGE,

    // USER MADE AN ERROR
    TRIP_ERROR,
};

typedef struct
{
    size_t len;
    unsigned char *buf;
} trip_recv_t;

/* TRiP Forward-Declared Handles */
typedef struct
{
    enum trip_action_e type;
    uint64_t now;
    union
    {
        int wait;
        bool send_ready;
        trip_recv_t recv;
        const char *err;
    } u;
} trip_action_t;

typedef struct
{
    void *data;
    int maxconn;
    int maxstream;
    int maxcredit;
    bool send_ready;
    hitime_t timeouts;
} trip_t;
typedef trip_t * Trip;

typedef struct
{
    void *data;
    size_t ilen;
    unsigned char *info;
    trip_t *router;
} trip_connection_t;

typedef struct
{
    void *data;
    trip_connection_t *connection;
} trip_stream_t;


void
trip_init(Trip);
void
trip_destroy(Trip);

void
trip_set(Trip, enum trip_option_e, void *);
void
trip_act(Trip, trip_action_t *);

uint64_t
trip_now(void);


#ifdef __cplusplus
}
#endif
#endif /* TRIP_H_ */

