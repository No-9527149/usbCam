#ifndef CAMERA_CPP_UTILS_H
#define CAMERA_CPP_UTILS_H

#include <SDL2/SDL.h>
#include <csetjmp>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <jpeglib.h>
#include <linux/videodev2.h>

#define NB_BUFFER 4

struct videoDev {
    int fd;
    char dev_Name[64];
    struct v4l2_capability cap;
    struct v4l2_fmtdesc fmt_Desc;
    struct v4l2_format fmt;
    struct v4l2_buffer buf;
    struct v4l2_requestbuffers re_Buf;
    void *mem[NB_BUFFER];
    unsigned char *raw_Buf;
    int raw_W;
    int raw_H;
    int raw_Format;
    int raw_Size;
    unsigned char *rgb_Buf;
    int rgb_W;
    int rgb_H;
    int rgb_Size;
    int is_Streaming;
};

struct errorMessage {
    struct jpeg_error_mgr pub;
    jmp_buf setJumpBuf;
};

typedef struct errorMessage *errorMessagePtr;

int playStream(struct videoDev *vd);
int stopStream(struct videoDev *vd);
int openVideoDevice(struct videoDev *vd);
int closeVideoDevice(struct videoDev *vd);
int grabFrame(struct videoDev *vd);

METHODDEF(void)
errorExit(j_common_ptr cinfo);

int jpegDecoder(unsigned char *jpgPtr, int jpgLen, unsigned char *rgbPtr);

void SDLFree();
int SDLInit(int video_W, int video_H);
void SDLDisplay(unsigned char *buf, int width, int height);

#endif
