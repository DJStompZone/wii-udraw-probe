#include <wiiuse/wpad.h>

#include "app.h"
#include "probe.h"
#include "ui.h"

static int s_should_quit = 0;
static probe_state_t s_probe;

void app_init(void) {
    WPAD_Init();
    probe_init(&s_probe);
}

void app_update(void) {
    WPAD_ScanPads();
    if (WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) {
        s_should_quit = 1;
    }
    probe_update(&s_probe);
}

void app_render(void) {
    ui_render(&s_probe);
}

int app_should_quit(void) {
    return s_should_quit;
}
