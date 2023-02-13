#ifndef _FFMPEG_CLIENT_H_
#define _FFMPEG_CLIENT_H_

#define handle int

// error code
#define FFMPEG_CLIENT_OK    0x00000000
#define FFMPEG_CLIENT_PARAM 0x80000001

typedef enum _ffmpeg_client_callback_type_
{
    ffmpeg_client_data_type,
    ffmpeg_client_msg_type,
} ffmpeg_client_callback_type;

typedef enum _ffmpeg_client_protocol_type_
{
    ffmpeg_client_rtsp_type,
    ffmpeg_client_rtmp_type,
    ffmpeg_client_http_type,
    ffmpeg_client_file_type
} ffmpeg_client_protocol_type;

typedef enum _ffmpeg_client_rtsp_transport_
{
    ffmpeg_client_udp_type,
    ffmpeg_client_tcp_type
} ffmpeg_client_rtsp_transport;

typedef struct _ffmpeg_client_rtsp_config_
{
    ffmpeg_client_rtsp_transport rtsp_transport;
    unsigned int min_port;
    unsigned int max_port;
    unsigned int timeout;
    unsigned int queue_size;
    unsigned int buffer_size;
} ffmpeg_client_rtsp_config;

typedef struct _ffmpeg_client_config_
{
    ffmpeg_client_protocol_type protocol_type;
    void*                       config;
} ffmpeg_client_config;

void (*ffmpeg_client_callback)(handle h, ffmpeg_client_callback_type type, void* data, int len, void* user);

int ffmpeg_client_create(handle* p, char* url);

int ffmpeg_client_destroy(handle h);

int ffmpeg_client_setconfig(handle h, ffmpeg_client_config* config);

int ffmpeg_client_start(handle h);

int ffmpeg_client_stop(handle h);

int ffmpeg_client_setcallback(ffmpeg_client_callback_type callback, void* user);

#endif // _FFMPEG_CLIENT_H_