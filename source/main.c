#include <gccore.h>
#include <stdio.h>

#include "app.h"

static void *xfb = NULL;
static GXRModeObj *rmode = NULL;

static void video_init_console(void) {
    VIDEO_Init();
    rmode = VIDEO_GetPreferredMode(NULL);
    xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
    console_init(xfb, 20, 20, rmode->fbWidth, rmode->xfbHeight, rmode->fbWidth * VI_DISPLAY_PIX_SZ);
    VIDEO_Configure(rmode);
    VIDEO_SetNextFramebuffer(xfb);
    VIDEO_SetBlack(FALSE);
    VIDEO_Flush();
    VIDEO_WaitVSync();
    if (rmode->viTVMode & VI_NON_INTERLACE) {
        VIDEO_WaitVSync();
    }
}

int main(void) {
    video_init_console();
    app_init();

    while (!app_should_quit()) {
        app_update();
        printf("\x1b[2J");
        printf("uDraw Probe\n\n");
        printf("Scaffold build running. Press HOME to exit.\n");
        app_render();
        VIDEO_WaitVSync();
    }

    return 0;
}
