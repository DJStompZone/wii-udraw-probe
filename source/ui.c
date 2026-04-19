#include "ui.h"

#include <stdio.h>

static const char *status_badge(probe_status_t status) {
    switch (status) {
        case PROBE_STATUS_RAW_CAPTURED:
            return "LIVE";
        case PROBE_STATUS_EXTENSION_PRESENT:
            return "WAIT";
        case PROBE_STATUS_RAW_FALLBACK:
            return "FALLBACK";
        case PROBE_STATUS_NO_WIIMOTE:
        case PROBE_STATUS_NO_EXTENSION:
            return "CHECK";
        case PROBE_STATUS_INIT:
        default:
            return "INIT";
    }
}

static void print_range(const char *label, const udraw_observed_range_t *range) {
    if (!range->seen) {
        printf("  %-8s n/a\n", label);
        return;
    }
    printf("  %-8s %u .. %u\n", label, range->min_value, range->max_value);
}

static void print_yes_no(const char *label, int value) {
    printf("  %-18s %s\n", label, value ? "YES" : "NO");
}

static void print_capture_status(const capture_state_t *capture) {
    printf("  Recording          %s\n", capture->recording ? "ON" : "OFF");
    printf("  Samples            %u / %u\n", capture->sample_count, CAPTURE_MAX_SAMPLES);
    printf("  Overflowed         %s\n", capture->overflowed ? "YES" : "NO");
    printf("  Save status        %s\n", capture->last_save_status);
}

void ui_render(const probe_state_t *state) {
    printf("uDraw Probe\n");
    printf("===========\n");
    printf("Connect a Wiimote and attach the THQ uDraw tablet.\n");
    printf("Press + to search for guest Wiimotes if needed.\n\n");

    printf("STATUS [%s] %s\n\n", status_badge(state->status), state->status_text);

    printf("IDENTITY\n");
    printf("  Expected ID        %02x %02x %02x %02x %02x %02x\n",
           UDRAW_ID_0, UDRAW_ID_1, UDRAW_ID_2, UDRAW_ID_3, UDRAW_ID_4, UDRAW_ID_5);
    printf("  Observed ID        %02x %02x %02x %02x %02x %02x\n",
           state->extension_id.bytes[0], state->extension_id.bytes[1], state->extension_id.bytes[2],
           state->extension_id.bytes[3], state->extension_id.bytes[4], state->extension_id.bytes[5]);
    print_yes_no("ID matches", state->id_valid);

    printf("\nRAW REPORT\n");
    printf("  Bytes              %02x %02x %02x %02x %02x %02x\n",
           state->raw_report.bytes[0], state->raw_report.bytes[1], state->raw_report.bytes[2],
           state->raw_report.bytes[3], state->raw_report.bytes[4], state->raw_report.bytes[5]);
    printf("  Source             %s\n",
           state->status == PROBE_STATUS_RAW_CAPTURED ? "live WPAD expansion bytes" : "placeholder scaffold bytes");

    printf("\nDECODED\n");
    printf("  Pen X              %u\n", state->decoded.pen_x);
    printf("  Pen Y              %u\n", state->decoded.pen_y);
    printf("  Pressure           %u\n", state->decoded.pressure);
    printf("  Upper button       %s\n", state->decoded.upper_button ? "PRESSED" : "released");
    printf("  Lower button       %s\n", state->decoded.lower_button ? "PRESSED" : "released");
    print_yes_no("Pen in range", state->decoded.pen_in_range);
    print_yes_no("Pen lifted", state->decoded.stylus_lifted);

    printf("\nOBSERVED MIN/MAX\n");
    print_range("X", &state->stats.x);
    print_range("Y", &state->stats.y);
    print_range("Pressure", &state->stats.pressure);

    printf("\nCAPTURE\n");
    print_capture_status(&state->capture);

    printf("\nCONTROLS\n");
    printf("  A      Start/stop capture\n");
    printf("  B      Save capture to SD card\n");
    printf("  1      Clear capture + reset min/max\n");
    printf("  +      Search for guest Wiimotes\n");
    printf("  HOME   Exit app\n");

    printf("\nSTATUS GUIDE\n");
    printf("  LIVE     Real unknown-extension bytes are being used.\n");
    printf("  WAIT     Extension is present, but raw bytes are not exposed yet.\n");
    printf("  FALLBACK Placeholder bytes are shown so decode/UI stays visible.\n");
    printf("  CHECK    Connect a Wiimote or extension and try again.\n");

    printf("\nNOTES\n");
    printf("  Decode assumptions follow the WiiBrew/Dolphin uDraw 6-byte model.\n");
    printf("  SD capture is optional and intended for real-hardware debugging.\n");
    printf("  Saved captures go to sd:/udraw_probe_capture.txt\n");
}
