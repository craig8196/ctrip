
#include <string.h>

#include "trip.h"
#include "trip_extra.h"
#include "hitime.h"


static void
_tripm_abort(bool is_malloc, size_t size)
{
    const char msg[] = "libtrp %s(%zu) failure";
    const char m[] = "malloc";
    const char r[] = "realloc";

    /* Note that 2**64 is actually 20 max chars, good to have space. */
    char buf[sizeof(msg) + sizeof(r) + 32];

    snprintf(buf, sizeof(buf), msg, is_malloc ? m : r, size);
    perror(buf);
    abort();
}

INLINE static void *
_tripm_alloc_impl(size_t size)
{
    void *mem = malloc(size);
    if (UNLIKELY(!mem))
    {
        _tripm_abort(true, size);
    }
    return mem;
}

INLINE static void *
_tripm_realloc_impl(void *oldmem, size_t size)
{
    void *mem = realloc(oldmem, size);
    if (UNLIKELY(!mem))
    {
        _tripm_abort(false, size);
    }
    return mem;
}

INLINE static void
_tripm_free_impl(void *mem)
{
    free(mem);
}

void
_tripm_zero_impl(void *mem, size_t len)
{
    memset(mem, 0, len);
}

static Trip _router = NULL;

void
trip_global_init(void)
{
    if (!_router)
    {
        _router = trip_new();
    }
}

Trip
trip_global_get(void)
{
    return _router;
}

void
trip_global_destroy(void)
{
    if (_router)
    {
        trip_free(&_router);
    }
}

Trip
trip_new(void)
{
    Trip r = tripm_alloc(sizeof(trip_t));
    trip_init(r);
    return r;
}

void
trip_free(Trip *r)
{
    trip_destroy(*r);
    tripm_free(*r);
    *r = NULL;
}

