#ifndef CAPTURE_H
#define CAPTURE_H

#include <stdbool.h>
#include <stdint.h>

#include "udraw.h"

#define CAPTURE_MAX_SAMPLES 512

typedef struct {
    uint32_t frame_index;
    udraw_raw_report_t raw_report;
    udraw_decoded_report_t decoded;
} capture_sample_t;

typedef struct {
    bool armed;
    bool recording;
    bool overflowed;
    bool ever_recorded;
    uint32_t next_frame_index;
    uint32_t sample_count;
    capture_sample_t samples[CAPTURE_MAX_SAMPLES];
    char last_save_path[64];
    char last_save_status[96];
} capture_state_t;

void capture_init(capture_state_t *state);
void capture_toggle_recording(capture_state_t *state);
void capture_clear(capture_state_t *state);
void capture_append(capture_state_t *state, const udraw_raw_report_t *raw, const udraw_decoded_report_t *decoded);
bool capture_save_to_sd(capture_state_t *state);

#endif
