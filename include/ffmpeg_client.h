#ifndef _FFMPEG_CLIENT_H_
#define _FFMPEG_CLIENT_H_

#define HANDLE int
#define INVALID_HANDLE (-1)

// error code
#define FFMPEG_OK         0x00000000
#define FFMPEG_ERR_PARAM  0x80000001
#define FFMPEG_ERR_MEMORY 0x80000002

typedef enum _ffmpeg_client_callback_type_ {
    ffmpeg_client_data_type = 0,
    ffmpeg_client_msg_type  = 1,
} ffmpeg_client_callback_type;

typedef enum _ffmpeg_client_protocol_type_ {
    ffmpeg_client_auto_type,
    ffmpeg_client_rtsp_type,
    ffmpeg_client_rtmp_type,
    ffmpeg_client_http_type,
    ffmpeg_client_file_type
} ffmpeg_client_protocol_type;

typedef enum _ffmpeg_client_rtsp_transport_ {
    ffmpeg_client_udp_type,
    ffmpeg_client_tcp_type
} ffmpeg_client_rtsp_transport;

typedef struct _ffmpeg_client_rtsp_param_ {
    ffmpeg_client_rtsp_transport rtsp_transport;
    unsigned int min_port;
    unsigned int max_port;
    unsigned int timeout;
    unsigned int queue_size;
    unsigned int buffer_size;
} ffmpeg_client_rtsp_param;

typedef struct _ffmpeg_client_config_ {
    ffmpeg_client_protocol_type protocol_type;
    void*                       param;
    unsigned int                len;
} ffmpeg_client_config;

typedef void (*ffmpeg_client_callback)(HANDLE h, ffmpeg_client_callback_type type, void *data, int len, void *user);

int ffmpeg_client_create(HANDLE *p, const char *url);

int ffmpeg_client_destroy(HANDLE h);

int ffmpeg_client_setconfig(HANDLE h, ffmpeg_client_config *config);

int ffmpeg_client_start(HANDLE h);

int ffmpeg_client_stop(HANDLE h);

int ffmpeg_client_setcallback(HANDLE h, ffmpeg_client_callback_type type, ffmpeg_client_callback callback, void *user);

#endif // _FFMPEG_CLIENT_H_