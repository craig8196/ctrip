
#include <stdbool.h>
#include <string.h>
#include "bdd.h"
#include "trip_all.h"


static Trip r = NULL;

spec("ctrip:basics")
{
    describe("create router")
    {
        before_each()
        {
            r = NULL;
            trip_global_init();
            r = trip_global_get();
            check(r);
        }

        after_each()
        {
            trip_global_destroy();
            r = NULL;
        }

        it("router not NULL")
        {
            Trip router = trip_new();
            check(router);
            trip_free(&router);
            check(!router);
        }

        it("create/destroy the global router")
        {
            check(r, "global variable is NULL");
            trip_global_destroy();
            r = trip_global_get();
            check(!r);
        }
    }

    describe("router no-op")
    {
        before_each()
        {
            r = NULL;
            trip_global_init();
            r = trip_global_get();
            check(r);
        }

        after_each()
        {
            trip_global_destroy();
            r = NULL;
        }

        it("returns the maximum timeout")
        {
            trip_action_t a;

            a.type = TRIP_TIMEOUT;

            trip_act(r, &a);

            check(TRIP_WAIT == a.type);
            check(INT_MAX == a.u.wait);
        }
    }

    describe("time operations")
    {
        it("gets the current time in ms (code-coverage)")
        {
            uint64_t now = trip_now();
            check(now);
        }
    }
}

