#include "udraw.h"

#include <stddef.h>

static void update_range(udraw_observed_range_t *range, uint16_t value) {
    if (!range->seen) {
        range->seen = true;
        range->min_value = value;
        range->max_value = value;
        return;
    }

    if (value < range->min_value) {
        range->min_value = value;
    }
    if (value > range->max_value) {
        range->max_value = value;
    }
}

void udraw_set_default_id(udraw_extension_id_t *id) {
    static const uint8_t expected[6] = {
        UDRAW_ID_0, UDRAW_ID_1, UDRAW_ID_2, UDRAW_ID_3, UDRAW_ID_4, UDRAW_ID_5,
    };
    for (size_t i = 0; i < 6; ++i) {
        id->bytes[i] = expected[i];
    }
}

bool udraw_id_matches_expected(const udraw_extension_id_t *id) {
    static const uint8_t expected[6] = {
        UDRAW_ID_0, UDRAW_ID_1, UDRAW_ID_2, UDRAW_ID_3, UDRAW_ID_4, UDRAW_ID_5,
    };
    for (size_t i = 0; i < 6; ++i) {
        if (id->bytes[i] != expected[i]) {
            return false;
        }
    }
    return true;
}

void udraw_decode_report(const udraw_raw_report_t *raw, udraw_decoded_report_t *decoded) {
    const uint8_t x_lo = raw->bytes[0];
    const uint8_t y_lo = raw->bytes[1];
    const uint8_t xy_hi = raw->bytes[2];

    decoded->pen_x = (uint16_t)x_lo | (uint16_t)((xy_hi & 0x0f) << 8);
    decoded->pen_y = (uint16_t)y_lo | (uint16_t)((xy_hi & 0xf0) << 4);
    decoded->pressure = raw->bytes[3];
    decoded->stylus_lifted = (raw->bytes[0] == 0xff && raw->bytes[1] == 0xff && raw->bytes[2] == 0xff);
    decoded->pen_in_range = !decoded->stylus_lifted;
    decoded->upper_button = (raw->bytes[5] & 0x01u) == 0;
    decoded->lower_button = (raw->bytes[5] & 0x02u) == 0;
    decoded->valid = true;
}

void udraw_stats_reset(udraw_runtime_stats_t *stats) {
    stats->x.seen = false;
    stats->y.seen = false;
    stats->pressure.seen = false;
}

void udraw_stats_update(udraw_runtime_stats_t *stats, const udraw_decoded_report_t *decoded) {
    if (!decoded->valid) {
        return;
    }

    update_range(&stats->pressure, decoded->pressure);
    if (!decoded->stylus_lifted) {
        update_range(&stats->x, decoded->pen_x);
        update_range(&stats->y, decoded->pen_y);
    }
}
