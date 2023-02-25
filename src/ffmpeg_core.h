#ifndef _FFMPEG_CORE_H_
#define _FFMPEG_CORE_H_

#include <map>
#include <mutex>
#include <thread>

#ifdef __cplusplus
extern "C" {
#endif
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavfilter/avfilter.h"
#include "libavutil/avutil.h"
#include "libavdevice/avdevice.h"
#ifdef __cplusplus
}
#endif

typedef enum _ffmpeg_core_callback_type_ {
    ffmpeg_core_data_type = 0,
    ffmpeg_core_msg_type  = 1,
} ffmpeg_core_callback_type;

typedef void (*ffmpeg_callback)(int h,
    ffmpeg_core_callback_type type, 
    void* data, int len, void* user);

typedef struct _ffmpeg_core_callback_info_ {
    void*           data_user;
    void*           msg_user;
    ffmpeg_callback data_callback;
    ffmpeg_callback msg_callback;
} ffmpeg_core_callback_info;

typedef struct _ffmpeg_core_context_ {
    std::string      url;
    std::thread      read_thread;
    AVDictionary    *dict;
    AVFormatContext *ifmt_ctx;
    AVFormatContext *ofmt_ctx;
    ffmpeg_core_callback_info callback;
} ffmpeg_core_context;

class FFmpegCore
{
public:
    FFmpegCore();
    FFmpegCore(std::string url);
    ~FFmpegCore();

public:
    int ffmpeg_core_init();
    int ffmpeg_core_setconfig(void *param, int len, void *protocol);
    int ffmpeg_core_start_stream(void *stream);
    int ffmpeg_core_stop_stream(void *stream);
    int ffmpeg_core_input_data(void *data, int len);
    int ffmpeg_core_regist_callback(ffmpeg_core_callback_type type, ffmpeg_callback func, void *user);
    int ffmpeg_core_create_handle();
    ffmpeg_core_context *ffmpeg_core_get_context();

    static int ffmpeg_core_release_handle(int h);
    static FFmpegCore* ffmpeg_core_instance(int h);

private:


private:
    ffmpeg_core_context               m_ctx;
    static std::mutex                 m_xhandle;
    static std::map<int, FFmpegCore*> m_mhandle;
};

#endif // _FFMPEG_CORE_H_