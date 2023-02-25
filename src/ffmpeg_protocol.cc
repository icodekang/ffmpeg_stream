#include "ffmpeg_protocol.h"
#include "ffmpeg_client.h"
#include "ffmpeg_trans.h"
#include "ffmpeg_core.h"
#include "spdlog/spdlog.h"

static int set_config_default(void *param, int len, void* user) {
    int ret = FFMPEG_OK;
    return ret;
}

static int rtsp_set_config(void *param, int len, void* user) {
    int ret = FFMPEG_OK;
    FFmpegCore *core = nullptr;
    ffmpeg_core_context *ctx = nullptr;
    ffmpeg_client_rtsp_param *p = nullptr;

    if (nullptr == param || len <= 0 || nullptr == user) {
        spdlog::error("param error, param={} len={} user={}", param, len, user);
        ret = FFMPEG_ERR_PARAM;
        goto fail;
    }

    p = (ffmpeg_client_rtsp_param*)param;
    if (nullptr == p) {
        spdlog::error("rtsp param is null, p={}", (void*)p);
        ret = FFMPEG_ERR_PARAM;
        goto fail;
    }

    core = (FFmpegCore*)user;
    if (nullptr == core) {
        spdlog::error("user param is null, core={}", (void*)core);
        ret = FFMPEG_ERR_PARAM;
        goto fail;
    }

    ctx = core->ffmpeg_core_get_context();

    av_dict_set(&ctx->dict, "initial_pause", "1", 0);
    switch (p->rtsp_transport) {
    case ffmpeg_client_udp_type:
        av_dict_set(&ctx->dict, "rtsp_transport", "udp", 0);
        break;
    case ffmpeg_client_tcp_type:
        av_dict_set(&ctx->dict, "rtsp_transport", "tcp", 0);
        break;
    default:
        av_dict_set(&ctx->dict, "rtsp_transport", "tcp", 0);
        break;
    }

    if (p->min_port)
        av_dict_set(&ctx->dict, "min_port", std::to_string(p->min_port).c_str(), 0);
    if (p->max_port)
        av_dict_set(&ctx->dict, "max_port", std::to_string(p->max_port).c_str(), 0);
    if (p->timeout)
        av_dict_set(&ctx->dict, "timeout", std::to_string(p->timeout).c_str(), 0);
    if (p->queue_size)
        av_dict_set(&ctx->dict, "reorder_queue_size", std::to_string(p->queue_size).c_str(), 0);
    if (p->buffer_size)
        av_dict_set(&ctx->dict, "buffer_size", std::to_string(p->buffer_size).c_str(), 0);

    return ret;

fail:
    return ret;
}

const ffmpeg_protocol ffmpeg_auto_protocol = {
    .name = "auto",
    .ffmpeg_set_config = set_config_default,
};

const ffmpeg_protocol ffmpeg_rtsp_protocol = {
    .name = "rtsp",
    .ffmpeg_set_config = rtsp_set_config,
};

const ffmpeg_protocol ffmpeg_rtmp_protocol = {
    .name = "rtmp",
    .ffmpeg_set_config = set_config_default,
};

const ffmpeg_protocol ffmpeg_http_protocol = {
    .name = "http",
    .ffmpeg_set_config = set_config_default,
};

const ffmpeg_protocol ffmpeg_file_protocol = {
    .name = "file",
    .ffmpeg_set_config = set_config_default,
};