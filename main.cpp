#include "utils/utils.h"
#include <unistd.h>
#include <cstdio>
#include <cstdlib>

struct videoDev vDev;

int main() {
    strcpy(vDev.dev_Name, "/dev/video2");
    vDev.raw_W = 3840;
    vDev.raw_H = 2160;
    vDev.raw_Format = V4L2_PIX_FMT_MJPEG;
    vDev.is_Streaming = 0;

    if (openVideoDevice(&vDev) < 0)
        return -1;
    if (playStream(&vDev) < 0)
        return -1;
    if (SDLInit(vDev.raw_W, vDev.raw_H) < 0)
        return -1;

    vDev.raw_Buf = (unsigned char *) calloc(1, vDev.raw_W * vDev.raw_H * 2);
    vDev.rgb_W = vDev.raw_W;
    vDev.rgb_H = vDev.raw_H;
    vDev.rgb_Size = vDev.rgb_W * vDev.rgb_H * 4;
    vDev.rgb_Buf = (unsigned char *) calloc(1, vDev.rgb_Size);

    int imgCount = 0;
    char fileName[100];
    FILE *imgFile;

    while (true) {
        if (grabFrame(&vDev) < 0) {
            sleep(10);
            continue;
        }

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_KEYDOWN:
                    printf("Key %s Down! Snap image no.%d\n", SDL_GetKeyName(event.key.keysym.sym), imgCount);
                    sprintf(fileName, "../image/img_%d.jpg", imgCount++);
                    imgFile = fopen(fileName, "wb");
                    std::fwrite(vDev.raw_Buf, sizeof(&vDev.raw_Buf), vDev.raw_W * vDev.raw_H * 2, imgFile);
                    fclose(imgFile);
                            break;
                case SDL_QUIT:
                    goto ExitApp;
            }
        }

        jpegDecoder(vDev.raw_Buf, vDev.raw_Size, vDev.rgb_Buf);
        SDLDisplay(vDev.rgb_Buf, vDev.rgb_W, vDev.rgb_H);
    }
ExitApp:
    SDLFree();
    stopStream(&vDev);
    closeVideoDevice(&vDev);
    free(vDev.raw_Buf);
    free(vDev.rgb_Buf);
    return 0;
}
