#include <gccore.h>
#include <ogcsys.h>
#include <stdio.h>

#include "app.h"

static void *xfb = NULL;
static GXRModeObj *rmode = NULL;

static void video_init_console(void) {
    VIDEO_Init();
    rmode = VIDEO_GetPreferredMode(NULL);
    xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
    CON_Init(xfb, 20, 20, rmode->fbWidth - 20, rmode->xfbHeight - 20, rmode->fbWidth * VI_DISPLAY_PIX_SZ);
    VIDEO_Configure(rmode);
    VIDEO_SetNextFramebuffer(xfb);
    VIDEO_ClearFrameBuffer(rmode, xfb, COLOR_BLACK);
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
        printf("\x1b[2;0H");
        app_update();
        app_render();
        VIDEO_WaitVSync();
    }

    return 0;
}
