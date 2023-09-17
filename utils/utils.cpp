#include "utils.h"
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

SDL_Window *gWindow = nullptr;
SDL_Renderer *gRenderer = nullptr;
SDL_Texture *gTexture = nullptr;

/**
  * @brief  Open video stream
  * @note   VIDIOC_STREAMON
  * @param  vd  struct videoDev
  * @retval 0   If video stream opened
**/
int playStream(struct videoDev *vd) {
    int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    int ret;

    ret = ioctl(vd->fd, VIDIOC_STREAMON, &type);
    if (ret < 0) {
        printf("Error: Unable to start capture\n");
        return ret;
    }
    vd->is_Streaming = 1;
    return 0;
}

/**
  * @brief  Close video stream
  * @note   VIDIOC_STREAMOFF
  * @param  vd  struct videoDev
  * @retval 0   If video stream closed
**/
int stopStream(struct videoDev *vd) {
    int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    int ret;

    ret = ioctl(vd->fd, VIDIOC_STREAMOFF, &type);
    if (ret < 0) {
        printf("Error: Unable to stop capture\n");
        return ret;
    }
    vd->is_Streaming = 0;
    return 0;
}

/**
  * @brief  Open video device
  * @note   1. Open video device: O_RDWR
  * @note   2. Query capability: VIDIOC_QUERYCAP && VIDEO_CAPTURE
  * @note   3. Query format: VIDIOC_ENUM_FMT
  * @param  vd  struct videoDev
  * @retval 0   If video device opened
**/
int openVideoDevice(struct videoDev *vd) {
    int i, ret;

    // open video device
    if ((vd->fd = open(vd->dev_Name, O_RDWR)) == -1) {
        printf("Error opening V4L interface\n");
        return -1;
    }

    //query capability
    memset(&vd->cap, 0, sizeof(struct v4l2_capability));
    ret = ioctl(vd->fd, VIDIOC_QUERYCAP, &vd->cap);
    if (ret < 0) {
        printf("Error: Unable to query device capability\n");
        return -1;
    }
    printf("Cap.driver..: %s\n", vd->cap.driver);
    printf("Cap.card....: %s\n", vd->cap.card);
    printf("Cap.bus_info: %s\n", vd->cap.bus_info);
    printf("Cap.version.: %u.%u.%u\n", (vd->cap.version >> 16) & 0xff, (vd->cap.version >> 8) & 0xff, vd->cap.version & 0xff);

    if ((vd->cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) == 0) {
        printf("Error: Video capture not supported\n");
        return -1;
    }

    // query format
    vd->fmt_Desc.index = 0;
    vd->fmt_Desc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    printf("Support Formats:\n");
    while (ioctl(vd->fd, VIDIOC_ENUM_FMT, &vd->fmt_Desc) != -1) {
        printf("%d. %s\n", vd->fmt_Desc.index + 1, vd->fmt_Desc.description);
        vd->fmt_Desc.index++;
    }

    // set format
    memset(&vd->fmt, 0, sizeof(struct v4l2_format));
    vd->fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    vd->fmt.fmt.pix.width = vd->raw_W;
    vd->fmt.fmt.pix.height = vd->raw_H;
    vd->fmt.fmt.pix.pixelformat = vd->raw_Format;
    vd->fmt.fmt.pix.field = V4L2_FIELD_ANY;
    ret = ioctl(vd->fd, VIDIOC_S_FMT, &vd->fmt);
    if (ret < 0) {
        printf("Error: Unable to set format\n");
        return -1;
    }

    // request buffers
    memset(&vd->re_Buf, 0, sizeof(struct v4l2_requestbuffers));
    vd->re_Buf.count = NB_BUFFER;
    vd->re_Buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    vd->re_Buf.memory = V4L2_MEMORY_MMAP;
    ret = ioctl(vd->fd, VIDIOC_REQBUFS, &vd->re_Buf);
    if (ret < 0) {
        printf("Error: Unable to allocate buffers\n");
        return -1;
    }

    // map the buffers
    for (i = 0; i < NB_BUFFER; i++) {
        memset(&vd->buf, 0, sizeof(struct v4l2_buffer));
        vd->buf.index = i;
        vd->buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        vd->buf.memory = V4L2_MEMORY_MMAP;
        ret = ioctl(vd->fd, VIDIOC_QUERYBUF, &vd->buf);
        if (ret < 0) {
            printf("Error: Unable to query buffer\n");
            return -1;
        }

        vd->mem[i] = mmap(nullptr, vd->buf.length, PROT_READ, MAP_SHARED, vd->fd, vd->buf.m.offset);
        if (vd->mem[i] == MAP_FAILED) {
            printf("Error: Unable to map buffer\n");
            return -1;
        }
    }

    // queue the buffers
    for (i = 0; i < NB_BUFFER; ++i) {
        memset(&vd->buf, 0, sizeof(struct v4l2_buffer));
        vd->buf.index = i;
        vd->buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        vd->buf.memory = V4L2_MEMORY_MMAP;
        ret = ioctl(vd->fd, VIDIOC_QBUF, &vd->buf);
        if (ret < 0) {
            printf("Error: Unable to query buffer\n");
            return -1;
        }
    }

    return 0;
}

/**
  * @brief  Close video device
  * @note   None
  * @param  vd  struct videoDev
  * @retval 0   If video device closed
**/
int closeVideoDevice(struct videoDev *vd) {
    if (vd->is_Streaming)
        stopStream(vd);
    close(vd->fd);
    return 0;
}

/**
  * @brief  Grab frame
  * @note   None
  * @param  vd  struct videoDev
  * @retval 0   If grab frame
**/
int grabFrame(struct videoDev *vd) {
#define HEADERFRAME1 0xaf
    int ret;
    if (!vd->is_Streaming)
        return -1;

    memset(&vd->buf, 0, sizeof(struct v4l2_buffer));
    vd->buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    vd->buf.memory = V4L2_MEMORY_MMAP;

    ret = ioctl(vd->fd, VIDIOC_DQBUF, &vd->buf);
    if (ret < 0) {
        printf("Error: Unable to dequeue buffer\n");
        return -1;
    }

    switch (vd->raw_Format) {
        case V4L2_PIX_FMT_MJPEG:
            if (vd->buf.bytesused == HEADERFRAME1) {
                printf("Ignoring empty buffer...\n");
                return -1;
            }

            memcpy(vd->raw_Buf, vd->mem[vd->buf.index], vd->buf.bytesused);
            vd->raw_Size = vd->buf.bytesused;
            break;
    }

    ret = ioctl(vd->fd, VIDIOC_QBUF, &vd->buf);
    if (ret < 0) {
        printf("Error: Unable to require buffer\n");
        return -1;
    }

    return 0;
}

/**
  * @brief  If exist error
  * @note   None
  * @param  cinfo   information
  * @retval None
**/
METHODDEF(void)
errorExit(j_common_ptr cinfo) {
    auto error = (errorMessagePtr) cinfo->err;
    (*cinfo->err->output_message)(cinfo);
    longjmp(error->setJumpBuf, 1);
}

/**
  * @brief  Decode jpeg
  * @note   None
  * @param  jpgPtr  unsigned char *
  * @param  jpgLen  int
  * @param  rgbPtr  unsigned char *
  * @retval 0       Id decode successful
**/
int jpegDecoder(unsigned char *jpgPtr, int jpgLen, unsigned char *rgbPtr) {
    struct jpeg_decompress_struct cinfo {};
    struct errorMessage jError {};

    cinfo.err = jpeg_std_error(&jError.pub);
    jError.pub.error_exit = errorExit;
    if (setjmp(jError.setJumpBuf)) {
        jpeg_destroy_decompress(&cinfo);
        return -1;
    }

    jpeg_create_decompress(&cinfo);
    jpeg_mem_src(&cinfo, jpgPtr, jpgLen);
    jpeg_read_header(&cinfo, true);
    jpeg_start_decompress(&cinfo);

    int row_stride = cinfo.output_width * cinfo.output_components;

    JSAMPARRAY buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

    // unsigned char *p = rgbPtr;
    int bytes = 0;

    while (cinfo.output_scanline < cinfo.output_height) {
        jpeg_read_scanlines(&cinfo, buffer, 1);

        memcpy(rgbPtr + bytes, buffer[0], row_stride);
        bytes += row_stride;
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    return 0;
}

/**
  * @brief  Free SDL
  * @note   None
  * @param  None
  * @retval None
**/
void SDLFree() {
    if (gTexture != nullptr) {
        SDL_DestroyTexture(gTexture);
        gTexture = nullptr;
    }
    if (gRenderer != nullptr) {
        SDL_DestroyRenderer(gRenderer);
        gRenderer = nullptr;
    }
    if (gWindow != nullptr) {
        SDL_DestroyWindow(gWindow);
        gWindow = nullptr;
    }
    SDL_Quit();
}

/**
  * @brief  Initial SDL
  * @note   None
  * @param  video_W
  * @param  video_H
  * @retval true    If successfully initial
**/
int SDLInit(int video_W, int video_H) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        goto SDLError;
    }

    gWindow = SDL_CreateWindow("Display", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, video_W, video_H / 2, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    if (!gWindow) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create SDL window: %s", SDL_GetError());
        goto SDLError;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, 0);
    if (!gRenderer) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create SDL renderer: %s", SDL_GetError());
        goto SDLError;
    }

    gTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, video_W, video_H);
    if (!gTexture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create SDL texture: %s", SDL_GetError());
        goto SDLError;
    }

    return true;
SDLError:
    SDLFree();
    return false;
}

/**
  * @brief  Display SDL
  * @note   None
  * @param  buffer  unsigned char *
  * @param  width   int
  * @param  height  int
  * @retval None
**/
void SDLDisplay(unsigned char *buffer, int width, int height) {
    SDL_UpdateTexture(gTexture, nullptr, buffer, width * 3);

    SDL_Rect srcRect;
    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = width;
    srcRect.h = height;
    SDL_Rect dstRect;
    dstRect.x = 0;
    dstRect.y = 0;
    dstRect.w = width;
    dstRect.h = height;

    SDL_RenderClear(gRenderer);
    SDL_RenderCopy(gRenderer, gTexture, &srcRect, &dstRect);
    SDL_RenderPresent(gRenderer);
}