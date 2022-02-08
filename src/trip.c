
#include <limits.h>

#include "hitime.h"
#include "trip.h"
#include "trip_protocol.h"
#include "trip_util.h"


void
trip_init(Trip r)
{
    tripm_zero(r, sizeof(*r));
    hitime_init(&r->timeouts);
}

void
trip_destroy(Trip r)
{
    hitime_destroy(&r->timeouts);
}

void
trip_set(Trip r, enum trip_option_e opt, void *data)
{
    switch (opt)
    {
        case TRIP_OPTION_MAXCONN:
            r->maxconn = *(int *)data;
        break;

        case TRIP_OPTION_MAXSTREAM:
            r->maxstream = *(int *)data;
        break;

        case TRIP_OPTION_MAXCREDIT:
            r->maxcredit = *(int *)data;
        break;

        default:
            // Do nothing with bogus option.
        break;
    }
}

void
trip_open(Trip r, trip_action_t *a, size_t len, unsigned char *buf)
{
    a->type = TRIP_RECV_REJECT;
    a->u.err = "open is unimplemented";
}

void
trip_seg(Trip r, trip_action_t *a, size_t len, unsigned char *buf)
{
    a->type = TRIP_RECV_REJECT;
    a->u.err = "segment is unimplemented";
}

void
trip_act(Trip r, trip_action_t *a)
{
    switch (a->type)
    {
        case TRIP_TIMEOUT:
        break;

        case TRIP_RECV:
        {
            size_t len = a->u.recv.len;
            unsigned char *buf = a->u.recv.buf;
            if (len < TRIPP_SEG_MIN)
            {
                a->type = TRIP_RECV_REJECT;
                a->u.err = "malformed: invalid length";
                return;
            }

            const uint8_t control = buf[0] & ~TRIPP_ENC_BIT;
            if (control > TRIPP_CTRL_MAX)
            {
                a->type = TRIP_RECV_REJECT;
                a->u.err = "malformed: invalid control";
                return;
            }

            if (TRIPP_CTRL_OPEN != control)
            {
                trip_seg(r, a, len, buf);
            }
            else
            {
                trip_open(r, a, len, buf);
            }
        }
        break;
#if 0
    /* OPEN segments must be unpacked by the router. */
    if (pre.control === control.OPEN) {
      if (!this._allowIncoming) {
        /* Only outgoing connections allowed. */
        this._reject(sourceKey, rinfo, reject.INCOMING);
        return;
      }

      if (!pre.encrypted && !this._allowUnsafeOpen) {
        /* Ugh. Not encrypted. */
        warn('Not encrypted:', rinfo);
        this._reject(sourceKey, rinfo, reject.UNSAFE);
        return;
      }

      debug('OPEN unpacking now.');
      const open = p.unOpen(seg, this._openKeys.publicKey, this._openKeys.secretKey);

      if (!open) {
        crit('Whycome no open?');
        /* Again. Still not enough information to report. Dropping segment. */
        this._reject(sourceKey, rinfo, reject.KEY);
        return;
      }

      debug('OPEN data: ', open);

      if (open.version !== version) {
        /* No way to verify sender. Reject with caution. */
        this._reject(sourceKey, rinfo, reject.VERSION);
        return;
      }

      let connection = this._getAddress(sourceKey);
      if (!connection) {
        if (!this._screenCb(pre.id, open.route, open.signatureBuffer, open.signature, rinfo)) {
          /* Cannot verify sender. Reject with caution. */
          this._reject(sourceKey, rinfo, reject.USER);
          return;
        }

        const newId = this._newId();
        if (!newId) {
          /* Cannot verify sender. Reject with caution. */
          this._reject(sourceKey, rinfo, reject.BUSY);
          return;
        }

        connection = mkConnection(
          this,
          newId,
          this._socket.mkSender(rinfo),
          {
            signKey: this._signKeys.secretKey,
            allowUnsafeSign: true
          }
        );

        if (!connection) {
          this._reject(sourceKey, rinfo, reject.SERVER);
          return;
        }

        this._setId(newId, connection);
        this._setAddress(sourceKey, connection);
        connection.challenge();
      }

      connection.handleOpen(pre, open);
    }
    else {
      /* Everything else gets routed to the connection. */
      if (!pre.encrypted && !this._allowUnsafeSegment) {
        /* Ugh. Not encrypted. */
        warn('Not encrypted:', pre, rinfo);
        this._reject(sourceKey, rinfo, reject.UNSAFE);
        return;
      }


      const connection = pre.id ? this._getId(pre.id) : null;
      if (connection) {
        connection.handleSegment(pre, seg);
      }
      else {
        /* Unverified sender. Reject with caution. */
        this._reject(sourceKey, rinfo, reject.ID);
        return;
      }
#endif
        break;

        case TRIP_SEND_READY:
        break;

        case TRIP_CONN:
        break;

        default:
            a->type = TRIP_ERROR;
        break;
    }

    if (a->type < TRIP_WAIT)
    {
        a->type = TRIP_WAIT;
        a->u.wait = hitime_get_wait(&r->timeouts);
    }
}

