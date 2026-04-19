#include <stdbool.h>

#include <wiiuse/wpad.h>

#include "app.h"
#include "probe.h"
#include "ui.h"

static int s_should_quit = 0;
static bool s_is_searching = false;
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
    if (down & WPAD_BUTTON_PLUS) {
        if (s_is_searching) {
            WPAD_StopSearch();
        } else {
            WPAD_Search();
        }
        s_is_searching = !s_is_searching;
    }
}

void app_init(void) {
    WPAD_Init();
    WPAD_SetDataFormat(WPAD_CHAN_ALL, WPAD_FMT_BTNS_ACC_IR);
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
