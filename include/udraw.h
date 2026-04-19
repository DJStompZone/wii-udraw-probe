#ifndef UDRAW_H
#define UDRAW_H

#include <stdbool.h>
#include <stdint.h>

#define UDRAW_REPORT_SIZE 6

#define UDRAW_ID_0 0xff
#define UDRAW_ID_1 0x00
#define UDRAW_ID_2 0xa4
#define UDRAW_ID_3 0x20
#define UDRAW_ID_4 0x01
#define UDRAW_ID_5 0x12

typedef struct {
    uint8_t bytes[6];
} udraw_extension_id_t;

typedef struct {
    uint8_t bytes[UDRAW_REPORT_SIZE];
} udraw_raw_report_t;

typedef struct {
    bool valid;
    bool stylus_lifted;
    bool pen_in_range;
    bool upper_button;
    bool lower_button;
    uint16_t pen_x;
    uint16_t pen_y;
    uint8_t pressure;
} udraw_decoded_report_t;

typedef struct {
    bool seen;
    uint16_t min_value;
    uint16_t max_value;
} udraw_observed_range_t;

typedef struct {
    udraw_observed_range_t x;
    udraw_observed_range_t y;
    udraw_observed_range_t pressure;
} udraw_runtime_stats_t;

void udraw_set_default_id(udraw_extension_id_t *id);
bool udraw_id_matches_expected(const udraw_extension_id_t *id);
void udraw_decode_report(const udraw_raw_report_t *raw, udraw_decoded_report_t *decoded);
void udraw_stats_reset(udraw_runtime_stats_t *stats);
void udraw_stats_update(udraw_runtime_stats_t *stats, const udraw_decoded_report_t *decoded);

#endif
