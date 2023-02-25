#ifndef _FFMPEG_TRANS_H_
#define _FFMPEG_TRANS_H_

#define HANDLE int
#define INVALID_HANDLE (-1)

#define FFMPEG_OK        0x00000000
#define FFMPEG_ERR_PARAM 0x80000001

typedef enum _ffmpeg_trans_callback_type {
    ffmpeg_trans_data_type = 0,
    ffmpeg_trans_msg_type  = 1,
} ffmpeg_trans_callback_type;

typedef void (*ffmpeg_trans_callback)(HANDLE h, ffmpeg_trans_callback_type type, void *data, int len, void *user);

int ffmpeg_trans_create(HANDLE *p);

int ffmpeg_trans_destroy(HANDLE h);

int ffmpeg_trans_setcallback(HANDLE h, ffmpeg_trans_callback_type type, ffmpeg_trans_callback callback, void *user);

#endif // _FFMPEG_TRANS_H_