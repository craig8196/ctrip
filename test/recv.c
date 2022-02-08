
#include <stdbool.h>
#include <string.h>
#include "bdd.h"
#include "trip_all.h"


trip_action_t _a;
trip_action_t *a = &_a;
static Trip r = NULL;

spec("ctrip:recv")
{
    describe("recv an open segment")
    {
        before_each()
        {
            a->type = TRIP_TIMEOUT;

            r = NULL;
            trip_global_init();
            r = trip_global_get();
            check(r);
        }

        after_each()
        {
            trip_global_destroy();
            r = NULL;
            check(!r);
        }

        it("should open a new connection")
        {
        }
    }

    describe("recv a segment")
    {
        before_each()
        {
            a->type = TRIP_TIMEOUT;

            r = NULL;
            trip_global_init();
            r = trip_global_get();
            check(r);
        }

        after_each()
        {
            trip_global_destroy();
            r = NULL;
            check(!r);
        }
    }

    describe("reject bad segment")
    {
        before_each()
        {
            a->type = TRIP_TIMEOUT;

            r = NULL;
            trip_global_init();
            r = trip_global_get();
            check(r);
        }

        after_each()
        {
            trip_global_destroy();
            r = NULL;
            check(!r);
        }

        it("reject too small a segment (white-box)")
        {
            a->type = TRIP_RECV;
            a->u.recv.len = 12;
            a->u.recv.buf = NULL;

            trip_act(r, a);

            check(TRIP_RECV_REJECT == a->type);
            check(a->u.err);
        }
    }
}

