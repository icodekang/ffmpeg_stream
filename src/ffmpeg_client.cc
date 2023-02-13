#include "ffmpeg_client.h"
#include "spdlog/spdlog.h"

int ffmpeg_client_create(handle* p, char* url)
{
    int ret = FFMPEG_CLIENT_OK;

    if (nullptr == p || nullptr == url) {
        spdlog::error("ffmpeg_client_create param, p={}, url={}", p, url);
        ret = FFMPEG_CLIENT_PARAM;
        goto fail;
    }

fail:
    return ret;
}

int ffmpeg_client_destroy(handle h)
{

}

int ffmpeg_client_setconfig(handle h, ffmpeg_client_config* config)
{

}

int ffmpeg_client_start(handle h)
{

}

int ffmpeg_client_stop(handle h)
{

}

int ffmpeg_client_setcallback(ffmpeg_client_callback_type callback, void* user)
{

}