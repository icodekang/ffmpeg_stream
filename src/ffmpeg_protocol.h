#ifndef _FFMPEG_PROTOCOL_H_
#define _FFMPEG_PROTOCOL_H_

typedef struct _ffmpeg_protocol_ {
    const char *name;
    int (*ffmpeg_set_config)(void *param, int len, void* user);
} ffmpeg_protocol;

extern const ffmpeg_protocol ffmpeg_auto_protocol;
extern const ffmpeg_protocol ffmpeg_rtsp_protocol;
extern const ffmpeg_protocol ffmpeg_rtmp_protocol;
extern const ffmpeg_protocol ffmpeg_http_protocol;
extern const ffmpeg_protocol ffmpeg_file_protocol;

#endif // _FFMPEG_PROTOCOL_H_