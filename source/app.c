#include <stdbool.h>

#include <wiiuse/wpad.h>

#include "app.h"
#include "probe.h"
#include "ui.h"

static int s_should_quit = 0;
static probe_state_t s_probe;

static void handle_controls(void) {
    const u32 down = WPAD_ButtonsDown(0);

    if (down & WPAD_BUTTON_HOME) {
        s_should_quit = 1;
    }
    if (down & WPAD_BUTTON_A) {
        capture_toggle_recording(&s_probe.capture);
    }
    if (down & WPAD_BUTTON_B) {
        capture_save_to_sd(&s_probe.capture);
    }
    if (down & WPAD_BUTTON_1) {
        capture_clear(&s_probe.capture);
        udraw_stats_reset(&s_probe.stats);
    }
}

void app_init(void) {
    WPAD_Init();
    probe_init(&s_probe);
}

void app_update(void) {
    WPAD_ScanPads();
    handle_controls();
    probe_update(&s_probe);
}

void app_render(void) {
    ui_render(&s_probe);
}

int app_should_quit(void) {
    return s_should_quit;
}
