#include "ui.h"

#include <stdio.h>

static void print_range(const char *label, const udraw_observed_range_t *range) {
    if (!range->seen) {
        printf("%s: n/a\n", label);
        return;
    }
    printf("%s: %u .. %u\n", label, range->min_value, range->max_value);
}

void ui_render(const probe_state_t *state) {
    printf("uDraw Probe\n");
    printf("===========\n\n");

    printf("Status: %s\n", state->status_text);
    printf("Expected extension ID: %02x %02x %02x %02x %02x %02x\n",
           UDRAW_ID_0, UDRAW_ID_1, UDRAW_ID_2, UDRAW_ID_3, UDRAW_ID_4, UDRAW_ID_5);
    printf("Observed extension ID: %02x %02x %02x %02x %02x %02x\n",
           state->extension_id.bytes[0], state->extension_id.bytes[1], state->extension_id.bytes[2],
           state->extension_id.bytes[3], state->extension_id.bytes[4], state->extension_id.bytes[5]);
    printf("ID matches expected: %s\n\n", state->id_valid ? "yes" : "no");

    printf("Raw 6-byte report: %02x %02x %02x %02x %02x %02x\n",
           state->raw_report.bytes[0], state->raw_report.bytes[1], state->raw_report.bytes[2],
           state->raw_report.bytes[3], state->raw_report.bytes[4], state->raw_report.bytes[5]);
    printf("Decoded pen X: %u\n", state->decoded.pen_x);
    printf("Decoded pen Y: %u\n", state->decoded.pen_y);
    printf("Decoded pen pressure: %u\n", state->decoded.pressure);
    printf("Upper pen button: %s\n", state->decoded.upper_button ? "pressed" : "released");
    printf("Lower pen button: %s\n", state->decoded.lower_button ? "pressed" : "released");
    printf("Pen in range: %s\n", state->decoded.pen_in_range ? "yes" : "no");
    printf("Pen lifted: %s\n\n", state->decoded.stylus_lifted ? "yes" : "no");

    printf("Observed runtime min/max\n");
    print_range("X", &state->stats.x);
    print_range("Y", &state->stats.y);
    print_range("Pressure", &state->stats.pressure);

    printf("\nNotes:\n");
    printf("- Current raw path is a scaffold using the WiiBrew/Dolphin report model.\n");
    printf("- Next step is replacing placeholder report bytes with real extension reads on hardware.\n");
    printf("- Press HOME on Wiimote to exit.\n");
}
