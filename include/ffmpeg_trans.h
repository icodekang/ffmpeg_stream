#ifndef _FFMPEG_TRANS_H_
#define _FFMPEG_TRANS_H_

#define handle int

typedef enum _ffmpeg_trans_callback_type_
{
    ffmpeg_trans_data_type,
    ffmpeg_trans_msg_type,
} ffmpeg_trans_callback_type;

void (*ffmpeg_trans_callback)(handle h, ffmpeg_trans_callback_type type, void* data, int len, void* user);

int ffmpeg_trans_create(handle* p);

int ffmpeg_trans_destroy(handle h);

int ffmpeg_trans_setcallback(ffmpeg_trans_callback_type callback, void* user);

#endif // _FFMPEG_TRANS_H_