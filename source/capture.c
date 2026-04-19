#include "capture.h"

#include <fat.h>
#include <stdio.h>
#include <string.h>

static bool ensure_fat_ready(void) {
    static bool attempted = false;
    static bool mounted = false;

    if (!attempted) {
        mounted = fatInitDefault();
        attempted = true;
    }

    return mounted;
}

void capture_init(capture_state_t *state) {
    memset(state, 0, sizeof(*state));
    snprintf(state->last_save_status, sizeof(state->last_save_status), "No capture saved yet");
}

void capture_toggle_recording(capture_state_t *state) {
    state->recording = !state->recording;
    state->armed = state->recording;
}

void capture_clear(capture_state_t *state) {
    state->recording = false;
    state->armed = false;
    state->overflowed = false;
    state->ever_recorded = false;
    state->next_frame_index = 0;
    state->sample_count = 0;
    state->last_save_path[0] = '\0';
    snprintf(state->last_save_status, sizeof(state->last_save_status), "Capture buffer cleared");
}

void capture_append(capture_state_t *state, const udraw_raw_report_t *raw, const udraw_decoded_report_t *decoded) {
    if (!state->recording) {
        return;
    }

    if (state->sample_count >= CAPTURE_MAX_SAMPLES) {
        state->overflowed = true;
        state->recording = false;
        snprintf(state->last_save_status, sizeof(state->last_save_status),
                 "Capture buffer full at %u samples", state->sample_count);
        return;
    }

    capture_sample_t *sample = &state->samples[state->sample_count++];
    sample->frame_index = state->next_frame_index++;
    sample->raw_report = *raw;
    sample->decoded = *decoded;
    state->ever_recorded = true;
}

bool capture_save_to_sd(capture_state_t *state) {
    if (!state->ever_recorded || state->sample_count == 0) {
        snprintf(state->last_save_status, sizeof(state->last_save_status), "Nothing recorded yet");
        return false;
    }

    if (!ensure_fat_ready()) {
        snprintf(state->last_save_status, sizeof(state->last_save_status), "SD init failed (fatInitDefault)");
        return false;
    }

    const char *path = "sd:/udraw_probe_capture.txt";
    FILE *fp = fopen(path, "w");
    if (!fp) {
        snprintf(state->last_save_status, sizeof(state->last_save_status), "Failed to open %s", path);
        return false;
    }

    fprintf(fp, "uDraw probe capture\n");
    fprintf(fp, "samples=%u\n", state->sample_count);
    fprintf(fp, "format=frame raw0 raw1 raw2 raw3 raw4 raw5 x y pressure upper lower in_range lifted\n");

    for (uint32_t i = 0; i < state->sample_count; ++i) {
        const capture_sample_t *sample = &state->samples[i];
        fprintf(fp,
                "%u %02x %02x %02x %02x %02x %02x %u %u %u %u %u %u %u\n",
                sample->frame_index,
                sample->raw_report.bytes[0], sample->raw_report.bytes[1], sample->raw_report.bytes[2],
                sample->raw_report.bytes[3], sample->raw_report.bytes[4], sample->raw_report.bytes[5],
                sample->decoded.pen_x, sample->decoded.pen_y, sample->decoded.pressure,
                sample->decoded.upper_button ? 1u : 0u,
                sample->decoded.lower_button ? 1u : 0u,
                sample->decoded.pen_in_range ? 1u : 0u,
                sample->decoded.stylus_lifted ? 1u : 0u);
    }

    fclose(fp);
    snprintf(state->last_save_path, sizeof(state->last_save_path), "%s", path);
    snprintf(state->last_save_status, sizeof(state->last_save_status), "Saved %u samples to %s",
             state->sample_count, path);
    return true;
}
