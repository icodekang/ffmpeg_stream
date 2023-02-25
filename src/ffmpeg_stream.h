#ifndef _FFMPEG_STREAM_H_
#define _FFMPEG_STREAM_H_

typedef struct _ffmpeg_stream_ {
    const char *name;
    int (*ffmpeg_stream_before)(void *user);
    int (*ffmpeg_stream_start)(void *user);
    int (*ffmpeg_stream_stop)(void *user);
    int (*ffmpeg_stream_after)(void *user);
} ffmpeg_stream;

extern const ffmpeg_stream ffmpeg_auto_stream;
extern const ffmpeg_stream ffmpeg_tran_stream;

#endif // _FFMPEG_STREAM_H_