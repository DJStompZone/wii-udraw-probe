#include <gccore.h>
#include <ogcsys.h>
#include <wiiuse/wpad.h>

#include "app.h"

static int s_should_quit = 0;

void app_init(void) {
    VIDEO_Init();
    WPAD_Init();
}

void app_update(void) {
    WPAD_ScanPads();
    u32 down = WPAD_ButtonsDown(0);
    if (down & WPAD_BUTTON_HOME) {
        s_should_quit = 1;
    }
}

void app_render(void) {
}

int app_should_quit(void) {
    return s_should_quit;
}
