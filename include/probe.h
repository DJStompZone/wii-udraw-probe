#ifndef PROBE_H
#define PROBE_H

#include <stdbool.h>

#include "udraw.h"

typedef enum {
    PROBE_STATUS_INIT = 0,
    PROBE_STATUS_NO_WIIMOTE,
    PROBE_STATUS_NO_EXTENSION,
    PROBE_STATUS_EXTENSION_PRESENT,
    PROBE_STATUS_RAW_FALLBACK,
} probe_status_t;

typedef struct {
    probe_status_t status;
    bool id_valid;
    bool report_valid;
    udraw_extension_id_t extension_id;
    udraw_raw_report_t raw_report;
    udraw_decoded_report_t decoded;
    udraw_runtime_stats_t stats;
    char status_text[96];
} probe_state_t;

void probe_init(probe_state_t *state);
void probe_update(probe_state_t *state);

#endif
