#include "probe.h"

#include <stdio.h>
#include <string.h>

#include <wiiuse/wpad.h>

static void set_status(probe_state_t *state, probe_status_t status, const char *message) {
    state->status = status;
    snprintf(state->status_text, sizeof(state->status_text), "%s", message);
}

static void fill_placeholder_report(probe_state_t *state, int tick) {
    state->raw_report.bytes[0] = (uint8_t)(0x56 + (tick % 32));
    state->raw_report.bytes[1] = (uint8_t)(0x65 + (tick % 48));
    state->raw_report.bytes[2] = (uint8_t)(0x75 | ((tick & 0x03) << 6));
    state->raw_report.bytes[3] = (uint8_t)(0x20 + (tick % 180));
    state->raw_report.bytes[4] = 0xff;
    state->raw_report.bytes[5] = 0xfb;
}

static void capture_id_from_type(probe_state_t *state, u32 type) {
    if (type == WPAD_EXP_UNKNOWN) {
        udraw_set_default_id(&state->extension_id);
        state->id_valid = true;
        return;
    }

    memset(&state->extension_id, 0, sizeof(state->extension_id));
    state->id_valid = false;
}

static bool capture_report_from_wpad_data(probe_state_t *state) {
    const WPADData *data = WPAD_Data(0);
    if (data == NULL) {
        return false;
    }

    if ((data->data_present & WPAD_DATA_EXPANSION) == 0) {
        return false;
    }

    if (data->exp.type != WPAD_EXP_UNKNOWN) {
        return false;
    }

    memcpy(state->raw_report.bytes, &data->exp, sizeof(state->raw_report.bytes));
    return true;
}

void probe_init(probe_state_t *state) {
    memset(state, 0, sizeof(*state));
    udraw_set_default_id(&state->extension_id);
    udraw_stats_reset(&state->stats);
    set_status(state, PROBE_STATUS_INIT, "Initializing WPAD");
}

void probe_update(probe_state_t *state) {
    static int tick = 0;
    ++tick;

    u32 type = WPAD_EXP_NONE;
    const s32 probe_result = WPAD_Probe(0, &type);

    state->report_valid = false;

    if (probe_result == WPAD_ERR_NO_CONTROLLER) {
        set_status(state, PROBE_STATUS_NO_WIIMOTE, "No Wiimote on channel 0");
        memset(&state->decoded, 0, sizeof(state->decoded));
        return;
    }

    if (probe_result != WPAD_ERR_NONE) {
        set_status(state, PROBE_STATUS_NO_WIIMOTE, "Wiimote not ready yet");
        memset(&state->decoded, 0, sizeof(state->decoded));
        return;
    }

    capture_id_from_type(state, type);

    if (type == WPAD_EXP_NONE) {
        set_status(state, PROBE_STATUS_NO_EXTENSION, "Wiimote connected, no extension detected");
        memset(&state->decoded, 0, sizeof(state->decoded));
        return;
    }

    if (capture_report_from_wpad_data(state)) {
        state->report_valid = true;
        set_status(state, PROBE_STATUS_RAW_CAPTURED, "Unknown extension detected, capturing raw WPAD expansion bytes");
        udraw_decode_report(&state->raw_report, &state->decoded);
        udraw_stats_update(&state->stats, &state->decoded);
        return;
    }

    if (type == WPAD_EXP_UNKNOWN) {
        set_status(state, PROBE_STATUS_EXTENSION_PRESENT, "Unknown extension detected, but raw WPAD bytes are not exposed yet");
    } else {
        set_status(state, PROBE_STATUS_RAW_FALLBACK, "Known libogc extension type, using placeholder raw decode path");
    }

    fill_placeholder_report(state, tick);
    state->report_valid = true;
    udraw_decode_report(&state->raw_report, &state->decoded);
    udraw_stats_update(&state->stats, &state->decoded);
}
