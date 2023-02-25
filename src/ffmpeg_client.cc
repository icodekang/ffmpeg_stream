#include "ffmpeg_client.h"
#include "ffmpeg_core.h"
#include "ffmpeg_protocol.h"
#include "ffmpeg_stream.h"
#include "spdlog/spdlog.h"

int ffmpeg_client_create(HANDLE *p, const char *url) {
    int ret = FFMPEG_OK;
    FFmpegCore *core = nullptr;

    if (nullptr == p || nullptr == url) {
        spdlog::error("ffmpeg_client_create param, p={}, url={}", (void*)p, url);
        ret = FFMPEG_ERR_PARAM;
        goto fail;
    }

    core = new FFmpegCore(url);
    if (nullptr == core) {
        spdlog::error("ffmpeg_client_create new memory failed");
        ret = FFMPEG_ERR_MEMORY;
        goto fail;
    }

    ret = core->ffmpeg_core_init();
    if (FFMPEG_OK != ret) {
        spdlog::error("ffmpeg_client_create ffmpeg_core_init failed, ret={}", ret);
        goto fail;
    }

    *p = core->ffmpeg_core_create_handle();
    return ret;

fail:
    if (core) {
        delete core;
        core = nullptr;
    }
    return ret;
}

int ffmpeg_client_destroy(HANDLE h) {
    int ret = FFMPEG_OK;

    ret = FFmpegCore::ffmpeg_core_release_handle(h);
    if (FFMPEG_OK != ret) {
        spdlog::error("ffmpeg_client_destroy ffmpeg_core_release_handle failed, ret={}", ret);
        goto fail;
    }
    return ret;

fail:
    return ret;
}

int ffmpeg_client_setconfig(HANDLE h, ffmpeg_client_config *config) {
    int ret = FFMPEG_OK;
    FFmpegCore *core = nullptr;

    if (nullptr == config) {
        spdlog::error("ffmpeg_client_setconfig param error, config={}", (void*)config);
        ret = FFMPEG_ERR_PARAM;
        goto fail;
    }

    core = FFmpegCore::ffmpeg_core_instance(h);
    if (nullptr == core) {
        spdlog::error("ffmpeg_client_setconfig ffmpeg_core_instance failed");
        ret = FFMPEG_ERR_PARAM;
        goto fail;
    }

    switch (config->protocol_type)  {
    case ffmpeg_client_auto_type:
        ret = core->ffmpeg_core_setconfig(config->param, config->len, (void*)&ffmpeg_auto_protocol);
        break;
    case ffmpeg_client_rtsp_type:
        ret = core->ffmpeg_core_setconfig(config->param, config->len, (void*)&ffmpeg_rtsp_protocol);
        break;
    case ffmpeg_client_rtmp_type:
        ret = core->ffmpeg_core_setconfig(config->param, config->len, (void*)&ffmpeg_rtmp_protocol);
        break;
    case ffmpeg_client_http_type:
        ret = core->ffmpeg_core_setconfig(config->param, config->len, (void*)&ffmpeg_http_protocol);
        break;
    case ffmpeg_client_file_type:
        ret = core->ffmpeg_core_setconfig(config->param, config->len, (void*)&ffmpeg_file_protocol);
        break;
    default:
        spdlog::error("ffmpeg_client_setconfig protocol type={} error", (int)config->protocol_type);
        ret = FFMPEG_ERR_PARAM;
        break;
    }
    return ret;

fail:
    return ret;
}

int ffmpeg_client_start(HANDLE h) {
    int ret = FFMPEG_OK;
    FFmpegCore *core = nullptr;

    core = FFmpegCore::ffmpeg_core_instance(h);
    if (nullptr == core) {
        spdlog::error("ffmpeg_client_setconfig ffmpeg_core_instance failed");
        ret = FFMPEG_ERR_PARAM;
        goto fail;
    }

    ret = core->ffmpeg_core_start_stream((void*)&ffmpeg_auto_stream);
    if (FFMPEG_OK != ret) {
        spdlog::error("ffmpeg_client_setconfig ffmpeg_core_start_stream failed, ret={}", ret);
        goto fail;
    }
    return ret;

fail:
    return ret;
}

int ffmpeg_client_stop(HANDLE h) {
    int ret = FFMPEG_OK;
    FFmpegCore *core = nullptr;

    core = FFmpegCore::ffmpeg_core_instance(h);
    if (nullptr == core) {
        spdlog::error("ffmpeg_client_setconfig ffmpeg_core_instance failed");
        ret = FFMPEG_ERR_PARAM;
        goto fail;
    }

    ret = core->ffmpeg_core_stop_stream((void*)&ffmpeg_auto_stream);
    if (FFMPEG_OK != ret) {
        spdlog::error("ffmpeg_client_setconfig ffmpeg_core_stop_stream failed, ret={}", ret);
        goto fail;
    }
    return ret;

fail:
    return ret;
}

int ffmpeg_client_setcallback(HANDLE h, ffmpeg_client_callback_type type, ffmpeg_client_callback callback, void *user) {
    int ret = FFMPEG_OK;
    FFmpegCore *core = nullptr;

    if (nullptr == callback) {
        spdlog::error("ffmpeg_client_setcallback param error, callback={}", (void*)callback);
        ret = FFMPEG_ERR_PARAM;
        goto fail;
    }

    core = FFmpegCore::ffmpeg_core_instance(h);
    if (nullptr == core) {
        spdlog::error("ffmpeg_client_setconfig ffmpeg_core_instance failed");
        ret = FFMPEG_ERR_PARAM;
        goto fail;
    }

    switch (type) {
    case ffmpeg_client_data_type:
        ret = core->ffmpeg_core_regist_callback(ffmpeg_core_data_type, (ffmpeg_callback)callback, user);
        break;
    case ffmpeg_client_msg_type:
        ret = core->ffmpeg_core_regist_callback(ffmpeg_core_msg_type, (ffmpeg_callback)callback, user);
        break;
    default:
        spdlog::error("ffmpeg_client_setcallback callback type={} error", (int)type);
        ret = FFMPEG_ERR_PARAM;
        break;
    }
    return ret;

fail:
    return ret;
}