
#include <stdint.h>
#include "hitime_extra.h"

#include "trip.h"



uint64_t
trip_now(void)
{
    return hitime_now_ms();
}

